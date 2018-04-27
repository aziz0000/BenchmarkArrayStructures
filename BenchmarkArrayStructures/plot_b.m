

inputfile='bm.txt';
%inputfile='bm_gcc.txt';

ff=fopen('bm_names.txt');
l = textscan(ff,'%s','delimiter','\n');
l = l{1};
fclose(ff);

d = dlmread(inputfile)';

lines  = {'o--';'^-.';'v--';'x-.'};
colors = winter(4);

figure;
for i = 1:4
    loglog(d(:,1),d(:,i+1),lines{i},'Color', colors(i,:));
    hold on;

end

legend(l, 'location','southeast');
grid on
%axis equal
axis square
