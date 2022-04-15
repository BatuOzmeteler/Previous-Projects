function [ q ] = JointConf2JointVec( configuration )
% q : joint vector
% configuration : configuration structure
for i=1:length(configuration)
    q(i)=configuration(i).JointPosition;
end

end

