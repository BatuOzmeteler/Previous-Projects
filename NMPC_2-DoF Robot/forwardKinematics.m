% Forward Kinematics
function xy = forwardKinematics(q)

    l1 = 0.5;
    l2 = 0.5;
    
    q1 = q(1);
    q2 = q(2);
    
    x = l1*cos(q1) + l2*cos(q1 + q2);
    y = l1*sin(q1) + l2*sin(q1 + q2);
    xy = [x; y];
    
end
