function translate_mri_data(filename, scale, output_data_type)
    % default that data is spread across the range of the data type
    % e.g. 0-255 for unsigned char
    if nargin < 2
        scale = false;
    end
    
    % output data type
    if nargin < 3
        output_data_type = 'float32';
    end
    
    data = read_mri_data(filename);
    
    if scale
        disp('scaling image...');
        data.image = scale_data(data.image, output_data_type);
    else
        disp('skipping image scaling...');
    end
    
    write_mri_data(filename, data, output_data_type);
end

function write_mri_data(input_filename, data, output_data_type)
    if ndims(data.image) == 3
        write_mhd_files(input_filename, data.image, data.resolution, output_data_type);
    elseif ndims(data.image) == 4
        for i = 1:size(data.image,4)
            filename_i = strcat(input_filename, num2str(i));
            write_mhd_files(filename_i, data.image(:,:,:,i), data.resolution, output_data_type);
        end
    end
    
end