require 'rake'
require 'fileutils'
include FileUtils::Verbose

task :default => [:make]

desc "Run segmentation"
task :run do
  sh "ruby -I ruby/lib ruby/bin/segment " +
     "rt0024_unmasked_tidy_rewrite_ADC.mhd " +
     "rt0024_unmasked_tidy_rewrite_ADC_threshold_level_set.mhd " +
     "rt0024_unmasked_tidy_rewrite_ADC_threshold.mhd " +
     "3 " + 
     "49 " +
     "70 " +
     "1"
     
  sh "say done"
end

desc "Compile itk code"
task :make do
  Dir.chdir('itk') { system("make") }
end

desc "Run refactored code and test output against original output"
task :run_refactor do
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

# String to label results folders with: Time.now.utc.strftime("%Y%m%d%H%M%S")