% In the binary files, the complete variables are written one at a time:
% e.g. the _EVECS file, the e1x array, containing the x component of the
% primary e-evector for the entire dataset is written first (256 x 256 x
% 256), then  e1y, then e1z.

function translate_mri_data(filename)
    % open file
    fid  = fopen(filename,'r');
    frewind(fid);
    
    % read header
    ncols   = fread(fid, 1, 'int16');
    nrows   = fread(fid, 1, 'int16');
    nslices = fread(fid, 1, 'int16');
    orientation_code = fread(fid, 1, 'int16');
    resolution = fread(fid, [1 3], 'float32');
    
    check_file_size_is_consistent(filename, [ncols, nrows, nslices]);
    
    % read binary data
    for islice = 1:nslices,
        img(:,:,islice) = fread(fid, [ncols, nrows],'float32');
    end
    
    data_type = 'char';
    
    img = scale_data(img, data_type);
    
    write_mhd_files(filename, img, resolution, data_type);
    
end

function check_file_size_is_consistent(filename, npixels)
    file_info = dir(filename);
    file_size = file_info.bytes;
    
    header_size = 20; % 4 int16s and 3 float32s
    body_size = prod(npixels) * 4; % number of pixels * 4 bytes in a float
    
    assert(file_size == header_size + body_size,...
           'Header size %d and body size %d do not add up to file size %d.',...
           header_size, body_size, file_size...
        );
    
end
