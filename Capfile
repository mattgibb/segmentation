require 'railsless-deploy'
load 'config/deploy'

desc "Generates 9 scalar images of eigenvectors from raw DTMRI data"
task :generate_eigenvectors do
  set :m_file_path, "/home/matg/imaging/segmentation/current/matlab"
  set :data_path, "/home/matg/imaging/images/Rat24/MRI/DTMRI/rt0024_unmasked_tidy_rewrite_D"
  run %{matlab -nodisplay -nosplash -nojvm -r "cd #{m_file_path}; try; generate_eigenvectors('#{data_path}'); catch e; disp(['Error: ' e.message]); end; pause(1); exit"}
end