function drawfigure(x,y,name,label_x,label_y)
semilogx(x,y);

%set figure envirement
grid on;
% legend('Simulated value',...
%     'theoretical value');
xlabel(label_x,'fontsize',15);
ylabel(label_y,'fontsize',15);
title(name,'fontsize',20);

%save figure
dot = findstr(name,'.');
if isempty(dot)
    name = strcat(name,'.png');
end;
% print('-dpng','r100',name);
set(gcf,'color',[1 1 1]);
F = getframe(gcf);
imwrite(F.cdata,name);
