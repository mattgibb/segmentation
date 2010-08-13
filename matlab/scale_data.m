function img = scale_data(img, data_type)
%scale_data spreads img across the data range of data_type
    
    old_min = min(img(:));
    old_max = max(img(:));
    new_min = 0;
    if(strcmp(data_type, 'char') || strcmp(data_type, 'uint8'))
        new_max = 255;
    elseif(strcmp(data_type, 'ushort') || strcmp(data_type, 'uint16'))
        new_max = 65535;
    else
        error('Data type not recognised when scaling data.');
    end
    
    % apply y = m*x + c
    c = - old_min;
    m = (new_max - new_min) / (old_max - old_min);
    
    img = m * img + c;
    
end