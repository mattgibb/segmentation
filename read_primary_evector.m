% In the binary files, the header starts with a line of text, followed by
% the x res, y res, z res, an orientation code, then an array containing
% the physical resolution (in mm) in x,y, and z. This is the same as the
% text files. However, in the binary files, the complete variables are
% written one at a time. So, for the _EVECS file, the e1x array, containing
% the x component of the primary e-evector for the entire dataset is
% written first (256 x 256 x 21), then e1y, then e1z.


ncol = 256;
nrow = 256;
nslice   = 21;

fid  = fopen('RT0006_03_D','r');

frewind(fid);

for islice = 1:nslice,

expr = sprintf('slice %.0f',islice);
dum1 = fread(fid, 1,'float32');
udat = fread(fid,[ncol nrow],'float32');
dum1 = fread(fid, 1,'float32');

contour(udat);
title(expr);
disp('Hit a key to continue ...'); pause;

end
