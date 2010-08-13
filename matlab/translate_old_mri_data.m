function data = translate_old_mri_data(filename)
%TRANSLATE_OLD_MRI_DATA reads data from mri files with strange headers
% This function works by seeing how many blocks of 256^3 * 4 can fit inside
% the file size, then sequentially reading them from the back of the file.
    
    % set dimensions
    nrows   = 256;
    ncols   = 256;
    nslices = 256;
    dimensions = [nrows ncols nslices];
    
    % open file
    fid  = fopen(filename,'r');
    frewind(fid);
    
    [scalars, header_size] = calculate_number_of_scalars_and_header_size(filename, dimensions);
    
    % skip the header
    fread(fid, header_size, 'int8');
    
    % read data into 4-D array
    for scalar = 1:scalars
        for slice = 1:nslices
            % read a slice
            data(:,:,slice,scalar) = fread(fid, [nrows ncols], 'float32');
        end
    end
    
    data_type = 'char';
    
    % stretch data across the full range of data_type
    data = scale_data(data, data_type);
    
    % write each component of the data in filename to a separate mhd file
    for scalar = 1:scalars
        new_filename = strcat( filename, '_', num2str(scalar) );
        write_mhd_files(new_filename, data(:,:,:,scalar), dimensions, data_type);
    end
    
end

function [scalars, header_size] = calculate_number_of_scalars_and_header_size(filename, dimensions)
    scalar_size = prod(dimensions) * 4;
    file_info = dir(filename);
    file_size = file_info.bytes;
    
    scalars = floor(file_size/scalar_size);
    header_size = mod(file_size, scalar_size);
    
    % print info to terminal
    str = sprintf('%s -> scalars: %d, header: %d', filename, scalars, header_size);
    disp(str);
end