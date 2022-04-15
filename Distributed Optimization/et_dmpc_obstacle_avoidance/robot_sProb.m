function sProb = robot_sProb(Nrobot,N,T,x0,xinit)
    %all-to-all coupling

    import casadi.*

    nx = 2; %states per robot
    nu = 2; %inputs per robot

    xmin = -inf(nx,1);
    xmax = inf(nx,1);
    umin = -2*ones(nu,1); %-inf?
    umax = 2*ones(nu,1);  %+inf?

    Ad = eye(2);
    Bd = T*eye(2);

    %setpoints
    xd = [6,8,8,6;
          6,6,8,8];

    % declare decision variables
    for i=1:Nrobot
        % states
        XX{i}  = SX.sym(['x' num2str(i)], [nx N]);
        % state copies
        for k=setdiff(1:Nrobot,i)
           ZZZ{i}{k} =  SX.sym(['z' num2str(i) num2str(k)], [nx N]);
           llbz{i}{k} = -inf(nx,N);
           uubz{i}{k} = inf(nx,N);
        end
        % inputs
        UU{i}  = SX.sym(['u' num2str(i)], [nu N]);
        % inital condition
        XX0{i} = SX.sym(['xx0' num2str(i)], [nx 1]);

    end

    % setup individual OCPs        
    for i=1:Nrobot
        JJ{i}   = 0;
        gg{i}   = [];
        hh{i}   = [];
        llbx{i} = [];
        uubx{i} = [];
        llbu{i} = [];
        uubu{i} = [];

        % over horizon ...

        % initial condition
    %     gg{i} = [gg{i}; XX{i}(:,1) - XX0{i}];
        gg{i} = [gg{i}; XX{i}(:,1) - x0{i}];
        % dynamics
        for j=1:N-1
            gg{i}      = [ gg{i}; XX{i}(:,j+1) - ( Ad*XX{i}(:,j) + Bd*UU{i}(:,j) )];
        end

        %terminal constraint
        gg{i} = [gg{i}; XX{i}(:,N)-xd(:,i)];

        for j= 1:N    
    %         JJ{i}      = JJ{i} + (XX{i}(:,j)-ppNum{i}(:,2))'*diag([1 1 1])*(XX{i}(:,j)-ppNum{i}(:,2)) ...
    %                            +  UU{i}(:,j)'*UU{i}(:,j);

            % state and input constraints
            llbx{i} = [ llbx{i}, xmin];
            uubx{i} = [ uubx{i}, xmax];
            llbu{i} = [ llbu{i}, umin];
            uubu{i} = [ uubu{i}, umax];
        end    
    end

    %cost
    C = -1/Nrobot*ones(Nrobot+1,Nrobot);
    for j = 1:Nrobot
       C(1,j) = 1/Nrobot; 
    end
    for i = 1:Nrobot
        C(i+1,i) = -1/Nrobot + 1;
    end
    Cp = kron(C,eye(nx));
    R = 1*eye(2);

    D = kron(blkdiag(1e-1,1e1*eye(Nrobot)),eye(nx));

    for i = 1:Nrobot
       ZZZ{i}{i} = XX{i};
       for k =  1:N
           tmp = [];
           for j = 1:Nrobot
               tmp = [tmp;ZZZ{i}{j}(:,k)]; %state + copies at time k
           end
    %       JJ{i} = JJ{i} + 0.5*(tmp-vertcat(xd(:))).'*(Cp.'*D*Cp)*(tmp-vertcat(xd(:))) + 0.5*UU{i}(:,k).'*R*UU{i}(:,k);
            JJ{i} = JJ{i} + 0.5*tmp.'*eye(numel(tmp))*tmp + 0.5*UU{i}(:,k).'*R*UU{i}(:,k);
       end
       clear tmp
    end

    for i=1:Nrobot
       ZZZ{i}{i} = XX{i};
    %    ZZZi      = vertcat(vertcat(ZZZ{i}{:}));   
       llbz{i}{i} = llbx{i};
       uubz{i}{i} = uubx{i};
       llbzi = vertcat(vertcat(llbz{i}{:}));
       uubzi = vertcat(vertcat(uubz{i}{:}));

       ZZZi = [];
       for j = 1:Nrobot
           if ~isempty(ZZZ{i}{j})
               for k = 1:size(ZZZ{i}{j},2)
                  ZZZi = [ZZZi; ZZZ{i}{j}(:,k)]; 
               end
           end       
       end
       UUi = [];
       for k = 1:size(UU{i},2)
           UUi = [UUi; UU{i}(:,k)];
       end
       XXU{i}    = [ ZZZi; UUi];
       llbxu{i} = [llbzi(:); llbu{i}(:)];
       uubxu{i} = [uubzi(:); uubu{i}(:)];
    end

    % for i = 1:Nrobot
    %     JJ{i} = JJ{i} + 0.1*XXU{i}.'*XXU{i};
    % end

    % set up consensus constraints
    AA = cell(1,Nrobot);
    for i = 1:Nrobot
        for j = setdiff(1:Nrobot,i)
            tmp = ZZZ{i}{j};
            if ~isempty(tmp) %i has copies of j
                ncopy = numel(tmp);
                nbefore  = numel(vertcat(vertcat(ZZZ{i}{1:j-1})));            
                nafter = numel(XXU{i})-ncopy-nbefore;
                AA{i} = [AA{i}; zeros(ncopy,nbefore),-eye(ncopy),zeros(ncopy,nafter)];

                nbefore = numel(vertcat(vertcat(ZZZ{j}{1:j-1})));
                nafter = numel(XXU{j})-ncopy-nbefore;
                AA{j} = [AA{j}; zeros(ncopy,nbefore),eye(ncopy),zeros(ncopy,nafter)];


                for k = setdiff(1:Nrobot,[i,j])
                    AA{k} = [AA{k}; zeros(ncopy,numel(XXU{k}))];
                end

    %            clear ncopy ZZZi nbefore nafter tmp
            end        
        end
    end

    % set up sProb
    % convert expressions to MATLAB functions
    % X0       = vertcat(XX0{:});

    for i=1:Nrobot
    %     sProb.locFuns.ffi{i} = Function(['f' num2str(i)],{XXU{i},XX0{i}},{JJ{i}});
    %     sProb.locFuns.ggi{i} = Function(['g' num2str(i)],{XXU{i},XX0{i}},{gg{i}});
    %     sProb.locFuns.hhi{i} = Function(['h' num2str(i)],{XXU{i},XX0{i}},{hh{i}});

        sProb.locFuns.ffi{i} = Function(['f' num2str(i)],{XXU{i}},{JJ{i}});
        sProb.locFuns.ggi{i} = Function(['g' num2str(i)],{XXU{i}},{gg{i}});
        sProb.locFuns.hhi{i} = Function(['h' num2str(i)],{XXU{i}},{hh{i}});

        % set up dIP parameters
        sProb.llbx{i}  = llbxu{i};
        sProb.uubx{i}  = uubxu{i};
        sProb.AA{i}    = AA{i};

        if isempty(xinit)
            sProb.zz0{i}   = zeros(length(XXU{i}),1);
        else
            sProb.zz0{i} = xinit{i};
        end

        sProb.llam0{i} = zeros(size(AA{i},1),1);

    end

end