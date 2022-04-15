function [] = plotTaskSpaceTrajectory( tformx )
clf;
pd=tform2trvec(tformx);
rotd=tform2rotm(tformx);
plot3(pd(:,1),pd(:,2),pd(:,3),'k-');
grid on; hold on;
scale=0.3;
ex=reshape(rotd(:,1,:),3,size(tformx,3));
quiver3(pd(:,1),pd(:,2),pd(:,3),ex(1,:)',ex(2,:)',ex(3,:)',scale,'r-');
ey=reshape(rotd(:,2,:),3,size(tformx,3));
quiver3(pd(:,1),pd(:,2),pd(:,3),ey(1,:)',ey(2,:)',ey(3,:)',scale,'g-');
ez=reshape(rotd(:,3,:),3,size(tformx,3));
quiver3(pd(:,1),pd(:,2),pd(:,3),ez(1,:)',ez(2,:)',ez(3,:)',scale,'b-');
xlabel('x'); ylabel('y'); zlabel('z');
hold off;
axis equal;
end

