require 'rake'
require 'fileutils'
include FileUtils::Verbose

task :default => [:make]

desc "Compile itk code"
task :make do
  Dir.chdir('itk') { system("make") }
end

desc "Run segmentation"
task :segment => [:make] do
  sh "itk/Threshold " + 
     "Rat24 " + 
     "rt0024_unmasked_tidy_rewrite_ADC.mhd " + 
     "rt0024_unmasked_tidy_rewrite_ADC_threshold_level_set.mhd " + 
     "rt0024_unmasked_tidy_rewrite_ADC_threshold.mhd " + 
     "3 " + 
     "49 " + 
     "70"
     
  sh "say done"
end

desc "Generate vox file from segmentation"
task :generate_vox do
  sh "ruby -I ruby/lib ruby/bin/write_vox_file " +
     "Rat24 " +
     "rt0024_unmasked_tidy_rewrite_ADC_threshold_level_set.mhd"
end

desc "Copy voxel file to heart server"
task :upload_vox do
  sh "scp results/Rat24/segmentation/voxels.vox heart:imaging/results/"
end

desc "Mesh vox file on heart server"
task :generate_mesh do
  sh "ruby -I ruby/lib ruby/bin/generate_mesh " +
     "Rat24"
end

desc "Convert spm file to carp format"
task :spm2carp do
  sh "ruby -I ruby/lib ruby/bin/spm2carp " +
     "Rat24"
end

desc "Copy pts and elem files from heart server"
task :download_carp do
  sh "scp heart:imaging/results/mesh.{elem,pts} results/Rat24/segmentation/"
end

desc "Generate centroid file"
task :generate_centroids => [:make] do
  sh "itk/GenerateCentroids " +
     "Rat24"
end

desc "Extract primary Eigenvector to lon file"
task :extract_vectors => [:make] do
  sh "itk/ExtractVectors " +
     "Rat24 " + 
     "rt0024_unmasked_tidy_rewrite_EVECS_1 " +
     "mesh.centroids " +
     "mesh.lon "
end

desc "Convert lon file to cardioviz format"
task :carp2cardioviz do
  sh "ruby -I ruby/lib ruby/bin/carp2cardioviz " +
     "Rat24"
end

namespace :refactor do
  desc "Run refactored code and test output against original output"
  task :run do
  #   register_128(refactor_dir)
  #   Rake::Task[:test].invoke
  #   cp 'config/registration_parameters_128.yml', refactor_dir
  end
  # 
  desc "Test refactored output against original output"
  task :test do
  #   diff_output = `diff -r -x .DS_Store #{test_dir} #{refactor_dir}`
  #   if $?.success?
  #     `echo The refactoring worked\! | growlnotify Success\!`
  #     puts "\nrefactoring successful!"
  #     `say refactoring successful!`
  #   else
  #     `echo '#{diff_output}' | growlnotify The refactoring fucked something\.`
  #     puts "\nDifferences:"
  #     puts diff_output
  #    `say refactoring failed`
  #   end
  end
end
# String to label results folders with: Time.now.utc.strftime("%Y%m%d%H%M%S")