function data = read_mri_data(filename)
%READ_MRI_DATA Read Patrick Hales' "rewrite" format
%   Detailed explanation goes here

    % open file
    fid  = fopen(filename,'r');
    frewind(fid);
    
    % read header
    data.ncols   = fread(fid, 1, 'uint16');
    data.nrows   = fread(fid, 1, 'uint16');
    data.nslices = fread(fid, 1, 'uint16');
    data.orientation_code = fread(fid, 1, 'uint16');
    data.resolution = fread(fid, [1 3], 'float32', 'a');
    
    % check file size is consistent and calculate data dimension
    data.ncoords = data_dimension(filename, [data.ncols, data.nrows, data.nslices]);
    
    % read binary data
    for icoord = 1:data.ncoords
        for islice = 1:data.nslices
            data.image(:,:,islice,icoord) = fread(fid, [data.ncols, data.nrows],'float32', 'a');
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
