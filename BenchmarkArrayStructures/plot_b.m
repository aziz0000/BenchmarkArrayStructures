

inputfile='bm.txt';
%inputfile='bm_gcc.txt';

ff=fopen('bm_names.txt');
l = textscan(ff,'%s','delimiter','\n');
l = l{1};
fclose(ff);

d = dlmread(inputfile)';

lines  = {'o--';'^:';'v-';'x-'};
colors = cool(4);

%d(:,2:end) = d(:,2:end) ./ d(1,2);
figure;
%loglog(d(:,1),d(:,2),'o--k',d(:,1),d(:,3),'^:b',d(:,1),d(:,4),'v-.m',d(:,1),d(:,5),'x-k',d(:,1),d(:,6),'x-c');
for i = 1:4
    loglog(d(:,1),d(:,i+1),lines{i},'Color', colors(i,:));
    hold on;

end

legend(l, 'location','southeast');
grid on
%axis equal
axis square
