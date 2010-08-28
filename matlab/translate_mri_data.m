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
    
    % check file size is consistent and calculate data dimension
    ncoords = data_dimension(filename, [ncols, nrows, nslices]);
    
    % read binary data
    for icoord = 1:ncoords
        for islice = 1:nslices
            img(:,:,islice,icoord) = fread(fid, [ncols, nrows],'float32');
        end
    end
    
    img = scale_data(img, 'char');
    
    if ndims(img) == 3
        write_mhd_files(filename, img, resolution, 'char');
    elseif ndims(img) == 4
        for i = 1:size(img,4)
            filename_i = strcat(filename, num2str(i));
            write_mhd_files(filename_i, img(:,:,:,i), resolution, 'char');
        end
    end
end

function dimension = data_dimension(filename, npixels)
    file_info = dir(filename);
    file_size = file_info.bytes;
    
    scalar_size = prod(npixels) * 4; % number of pixels * 4 bytes in a float
    
    dimension = floor( file_size/scalar_size );
    header_size = mod(file_size,scalar_size);
    
    assert(header_size == 20,... % 4 int16s and 3 float32s
           'Header size doesn''t equal 20.'...
        );
    
end
