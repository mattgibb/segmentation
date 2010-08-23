require 'rake'
require 'fileutils'
include FileUtils::Verbose

task :default => [:make]

desc "Compile itk code"
task :make do
  Dir.chdir('itk') { system("make") }
end

desc "Run segmentation"
task :run => [:make] do
  sh "./Threshold " + 
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
task :vox do
  sh "ruby -I ruby/lib ruby/bin/write_vox_file " +
     "Rat24 " +
     "rt0024_unmasked_tidy_rewrite_ADC_threshold_level_set.mhd"
end

desc "Copy voxel file to heart server"
task :upload_vox do
  
end

desc "Mesh vox file on heart server and copy back the results"
task :generate_mesh do
  sh "ruby -I ruby/lib ruby/bin/generate_mesh " +
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