function WriteMetaFile(filename, img, resolution, data_type)

% construct file names
[pathstr,name]=fileparts(filename);
mhd = strcat(pathstr, '/', name, '.mhd');
raw = strcat(pathstr, '/', name, '.raw');


% write mhd file
fid=fopen(mhd, 'w');
assert(fid>0, 'Impossible to open file %s\n', mhd);

% dimensions
fprintf(fid, 'NDims = %d\n', ndims(img));

if(ndims(img)==2)
    fprintf(fid, 'DimSize = %d %d \n', size(img,1), size(img,2));
    fprintf(fid, 'ElementSpacing = %1.4f %1.4f\n', resolution(1), resolution(2));
    
elseif(ndims(img) == 3)
    fprintf(fid, 'DimSize = %d %d %d\n', size(img,1), size(img,2), size(img,3));
    fprintf(fid, 'ElementSpacing = %1.4f %1.4f %1.4f\n', resolution(1), resolution(2), resolution(3));

elseif(ndims(img)==4)
    fprintf(fid, 'DimSize = %d %d %d %d\n', size(img,1), size(img,2), size(img,3), size(img,4));
    fprintf(fid, 'ElementSpacing = %1.4f %1.4f %1.4f %1.4f\n', resolution(1), resolution(2), resolution(3), resolution(4));
     
end

% data type
if(strcmp(data_type, 'char') || strcmp(data_type, 'uint8'))
    fprintf(fid, 'ElementType = MET_UCHAR\n');
    
elseif(strcmp(data_type, 'short'))
    fprintf(fid, 'ElementType = MET_SHORT\n');
    
elseif(strcmp(data_type, 'float32'))
    fprintf(fid, 'ElementType = MET_FLOAT\n');
end

fprintf(fid, 'ElementByteOrderMSB = False\n');
fprintf(fid, 'ElementDataFile = %s\n', strcat(name, '.raw'));
fclose(fid);


% write raw file
fid=fopen(raw, 'w');
assert(fid>0, 'Impossible to open file %s\n', raw);

fwrite(fid, img, data_type);
fclose(fid);
