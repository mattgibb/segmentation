function generate_eigenvectors(tensor_filename)
%GENERATE_EIGENVECTORS Summary of this function goes here
%   Detailed explanation goes here
    data = read_mri_data(tensor_filename);
    
    % allocate 5-d tensor field
    tensors = zeros(3,3,data.ncols,data.nrows,data.nslices);
    
    % assign elements of 3x3 tensor
    tensors(1,1,:,:,:) = data.image(:,:,:,1); % xx
    tensors(2,2,:,:,:) = data.image(:,:,:,2); % yy
    tensors(3,3,:,:,:) = data.image(:,:,:,3); % zz
    tensors(1,2,:,:,:) = data.image(:,:,:,4); % xy
    tensors(2,1,:,:,:) = data.image(:,:,:,4); % xy
    tensors(1,3,:,:,:) = data.image(:,:,:,5); % xz
    tensors(3,1,:,:,:) = data.image(:,:,:,5); % xz
    tensors(2,3,:,:,:) = data.image(:,:,:,6); % yz
    tensors(3,2,:,:,:) = data.image(:,:,:,6); % yz
    
    % calculate eigenvectors in order of absolute magnitude
    % and overwrite tensors with them
    for slice = 1:data.nslices
        for row = 1:data.nrows
            for col = 1:data.ncols
                % extract eigenvalues and vectors
                tensor = tensors(:,:,col,row,slice);
                [vectors,values] = eig(tensor);
                
                % can't have negative diffusion coefficients
                assert(all(abs(values) == values), 'Matrix has negative eigenvalues');
                
                % can't have NaNs in vectors
                assert(~any(isnan(vectors(:)), 'Vectors cannot contain NaNs');
                
                % turn eigenvalue matrix into column vector
                values = diag(values);
                
                % sort eigenvectors in descending order of value magnitude
                [sorted_values,indeces] = sort(values);
                sorted_vectors = vectors(:,indeces);
                
                % assign sorted vectors over tensors
                % to save memory
                tensors(:,:,col,row,slice) = sorted_vectors;
            end
        end
    end
    
    % write 9 scalar mhd files
    for vector = 1:3
        for coordinate = 1:3
            filename_ij = strcat(tensor_filename,...
                                 num2str(vector),...
                                 num2str(coordinate));
            write_mhd_files(filename_ij,...
                            tensors(vector,coordinate,:,:,:),...
                            data.resolution,
                            'float32');
        end
    end
    
end