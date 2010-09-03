function translate_mri_data(filename, scale, data_type)
    % default that data is spread across the range of the data type
    % e.g. 0-255 for unsigned char
    if nargin < 2
        scale = true;
    end
    
    % output data type
    if nargin < 3
        data_type = 'char';
    end
    
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
    
    if scale
        disp('scaling image...');
        img = scale_data(img, 'char');
    else
        disp('skipping image scaling...');
    end
    
    if ndims(img) == 3
        write_mhd_files(filename, img, resolution, data_type);
    elseif ndims(img) == 4
        for i = 1:size(img,4)
            filename_i = strcat(filename, num2str(i));
            write_mhd_files(filename_i, img(:,:,:,i), resolution, data_type);
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
