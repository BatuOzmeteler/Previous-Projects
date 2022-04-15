function [ solADM ] = run_ADMM_nlp(sProb, opts)

import casadi.*

NsubSys = length(sProb.AA);
AA      = sProb.AA;
Ncons   = size(AA{1},1);

% set default options

sProb      = setDefaultVals(sProb);

opts.alg = 'ADMM';
opts.locSol = 'ipopt';
opts.solveQP = 'MA57';

%% build local subproblems and CasADi functions
% timers
totTimer   = tic;
setupTimer = tic;

rhoCas      = SX.sym('rho',1,1);
for i=1:NsubSys
    nx       = size(sProb.AA{i},2);
    iter.loc.xxCas        = SX.sym('x',nx,1);
    iter.zzCas            = SX.sym('z',nx,1);
    
    % local inequality constraints
    sProb.locFunsCas.ggi{i}  = sProb.locFuns.ggi{i}(iter.loc.xxCas);
    if ~isempty(sProb.locFuns.hhi{i})
        sProb.locFunsCas.hhi{i}  = sProb.locFuns.hhi{i}(iter.loc.xxCas);
    else
        sProb.locFunsCas.hhi{i} = [];
    end
    
    % output dimensions of local constraints
    nngi{i} = size(sProb.locFunsCas.ggi{i},1);
    nnhi{i} = size(sProb.locFunsCas.hhi{i},1);
                
    
    % set up bounds for equalities/inequalities
    lbg{i} = [zeros(nngi{i},1); -inf*ones(nnhi{i},1)];
    ubg{i} = zeros(nngi{i}+nnhi{i},1);
    
    % symbolic multipliers
    nuCas{i}   = SX.sym('nu',nx,1);
    
    % parameter vector of CasADi
    pCas        = [ rhoCas;
                    nuCas{i};
                    iter.zzCas];
                
    
    % objective function for local NLP's
    ffiLocCas = sProb.locFuns.ffi{i}(iter.loc.xxCas) + nuCas{i}'*iter.loc.xxCas ...
            + rhoCas/2*((iter.loc.xxCas - iter.zzCas)'*(iter.loc.xxCas - iter.zzCas));

    % set up local solvers
    nlp       = struct('x',iter.loc.xxCas,'f',ffiLocCas,'g',[sProb.locFunsCas.ggi{i}; sProb.locFunsCas.hhi{i}],'p',pCas);
    nnlp{i}   = nlpsol('solver',opts.locSol,nlp);

end
timers.setupT = toc(setupTimer);


%% build H and A for ctr. QP
A   = horzcat(AA{:});

% replacement with Identity matrix should also gain ADMM according to Yuning
nx  = size(horzcat(AA{:}),2);
HQP = opts.rho*eye(nx);


gam   = 0; %1e-3;
L     = diag(double(~sum(abs(A),1)));
HQP   = HQP + gam*L'*L;

%% ADMM iterations
% initialization

i                   = 1;
iter.loc.xx             = sProb.zz0;
iter.zz                 = sProb.zz0;
iter.logg.X = [vertcat(sProb.zz0{:})];
for j = 1:NsubSys
    nx       = size(sProb.AA{j},2);
    if ~isfield(sProb,'zz0')
        iter.loc.xx{j} = zeros(nx,1);
        iter.zz{j} = zeros(nx,1);
    else
        iter.loc.xx{j} = sProb.zz0{j};
        iter.zz{j}     = sProb.zz0{j};
    end
    if ~isfield(sProb,'nu')
        nu{j} = zeros(nx,1);
    else
        nu{j} = sProb.nu{j};
    end
end
iterTimer           = tic;
while i <= opts.maxiter% && norm(delx,inf)>eps   
    for j = 1:NsubSys
        % set up parameter vector for local NLP's
        pNum = [ opts.rho0;
                 nu{j};
                 iter.zz{j}];
                                   
        tic     
        % solve local NLP's
        if ~isempty(sProb.llbx{j}) %only call casadi with ubx,lbx if their not empty
            sol = nnlp{j}('x0' , iter.zz{j},...
                          'p',   pNum,...
                          'lbx', sProb.llbx{j},...
                          'ubx', sProb.uubx{j},...
                          'lbg', lbg{j}, ...
                          'ubg', ubg{j});
        else %if ubx,lbx are empty, then don't use them as arguments to casadi;
            %if you call casadi with empty ubx,lbx then it will insert some constraints for you
            sol = nnlp{j}('x0' , iter.zz{j},...
                          'p',   pNum,...
                          'lbg', lbg{j}, ...
                          'ubg', ubg{j});  
        end %todo: insert additional checks if ubx XOR lbx is empty
%         timers.NLPtotTime = timers.NLPtotTime + toc;   
                                    
        iter.loc.xx{j}  = full(sol.x);
%         kapp{j}         = full(sol.lam_g);                      
%         KioptEq{j}      = kapp{j}(1:nngi{j});
%         KioptIneq{j}    = kapp{j}(nngi{j}+1:end);
    end
    % gloabl x vector
    x = vertcat(iter.loc.xx{:});
         
    % Solve central QP
    hQP_T=[];
    for j=1:NsubSys
       hQP_T  = [hQP_T -opts.rho0*iter.loc.xx{j}'-nu{j}'];
    end
    hQP   = hQP_T';
    AQP   = A;
    bQP   = zeros(size(A,1),1);    
    
    % regularization only for components not involved in consensus and
    % project them back on x_k
    hQP   = hQP - gam*L'*L*x;
    
    % solve QP
    tic
%     [z, ~] = solveQP(HQP,hQP,AQP,bQP,opts.solveQP);
    qp_opt = optimoptions('quadprog','Display','off');
    [zp,~,~,~,lam] = quadprog(HQP,hQP,[],[],AQP,bQP,[],[],vertcat(iter.zz{:}),qp_opt);
%     timers.QPtotTime      = timers.QPtotTime + toc; 
    
    % divide into subvectors
    ctr   = 1;
    iter.zzOld = iter.zz;
    for j=1:NsubSys
        ni          = length(iter.zz{j});
        iter.zz{j}       = zp(ctr:(ctr+ni-1)); 
        ctr = ctr + ni;
    end
    
    % lambda update after z update
    for j = 1:NsubSys
         nu{j} = nu{j} + opts.rho*(iter.loc.xx{j}-iter.zz{j}); 
    end
    

    % Erseghe update parameter is 1.025 and starts with 2 fort IEEE 57?
%     if strcmp(opts.rhoUpdate,'true')
%         %rho = rho*1.01;
%         
%         % update rule according to Guo 17 from remote point
%         if norm(A*x,inf) > 0.9*norm(A*xOld,inf) && i > 1
%             rho = rho*1.025;
%         end
%     end
    
    % logging of variables?
%     loggFl = false;
%     if loggFl == true
%         logValues;
%     end
    
    iter.logg.X = [iter.logg.X, x];
            
    % plot iterates?
%     if strcmp(opts.plot,'true') 
%         tic
%         plotIterates;
%         timers.plotTimer = timers.plotTimer + toc;
%     end
    
    i = i+1;
end
timers.iterTime = toc(iterTimer);

% total time
timers.totTime  = toc(totTimer);
sol.timers = timers;

% display solver output and timing
% dispSummary(opts.maxiter, opts, iter);
% displayTimers(timers, opts);

solADM.logg   = iter.logg;
solADM.xxOpt  = iter.loc.xx;
solADM.nu = nu;

for i = 1:NsubSys
    solADM.llam{i} = lam;
end

end


function sProb = setDefaultVals(sProb)
% set functions to emptyfunctions if not present
NsubSys = length(sProb.AA);
Ncons   = size(sProb.AA{1},1);
emptyfun      = @(x) [];

% set ggi and hhi to emptyfun if not present
if ~isfield(sProb.locFuns, 'ggi')
    [sProb.locFuns.ggi{1:NsubSys}] = deal(emptyfun);
end
if ~isfield(sProb.locFuns, 'hhi')
    [sProb.locFuns.hhi{1:NsubSys}] = deal(emptyfun);
end

% set default primal values to zero if not set
if ~isfield(sProb, 'zz0')
    for i=1:NsubSys
        nxi          = size(sProb.AA{i},2);
        sProb.zz0{i} = zeros(nxi,1);
    end
end

% set default lower/upper bounds if not present
if ~isfield(sProb, 'llbx')
    for i=1:NsubSys
        nxi           = size(sProb.AA{i},2);        
        sProb.llbx{i} = -inf*ones(nxi,1);
    end
end

% set default lower/upper bounds if not present
if ~isfield(sProb, 'uubx')
    for i=1:NsubSys
        nxi           = size(sProb.AA{i},2);        
        sProb.uubx{i} = inf*ones(nxi,1);
    end
end

% set default multipliers to zero if not present
if ~isfield(sProb, 'lam0')
    sProb.lam0 = zeros(size(sProb.AA{1},1),1);
end

% set b to zero if not defined
if ~isfield(sProb, 'b')
    sProb.b    = zeros(Ncons,1);
end
end
