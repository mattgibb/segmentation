require 'rake'
require 'fileutils'
include FileUtils::Verbose

task :default => [:make]

desc "Compile itk code"
task :make do
  Dir.chdir('itk_build') { system("make") }
end

desc "Run segmentation"
task :segment => [:make] do
  sh "itk_build/Threshold " + 
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
  # voxmesher conf_file number_of_processors
  command = "/users/martb/Tarantula2.1/bin/linux/voxmesher \
             imaging/segmentation/current/config/tara.conf \
             8"

  # run tarantula
  system "ssh heart #{command}"

  puts "The only parameter in the tara.conf file which you may want
        to vary is the BASEH number. This must be a power of 2 
        (i.e. 2,4,8,16,32,64 etc). The smaller the number, the
        finer your mesh (i.e. more nodes/elements)."
end

desc "Convert spm file to carp format"
task :spm2carp do
  sh "ruby -I ruby/lib ruby/bin/spm2carp " +
     "Rat24"
end

desc "copy mesh files from heart to Martin's computer"
task :transfer_mesh do
  system "ssh martin 'scp heart:imaging/results/mesh.* imaging/results'"
end

desc "Extract tissue mesh from tissue-bath mesh"
task :extract_tissue do
  command = "'cd imaging/results; " +
            "/home/scratch/programmes/CARP/CARP_local/bin/carpm.linux.petsc " + # carp executable
            "-experiment 3 " + # dont bother running simulation, just output tissue mesh
            "-meshname /users/matg/imaging/results/mesh " + # input mesh name
            "-gridout_i 2 " + # output all mesh files
            "-simID .'" # output directory
  
  system "ssh martin #{command}"
end

desc "Copy pts and elem files from heart server"
task :download_carp do
  # -a archive, -z compressed, -c skip based on checksum, -v verbose,
  # -P partial progress, -h human readable
  sh "rsync -azcvPh martin:imaging/results/* results/Rat24/segmentation/"
end

desc "Generate centroid file"
task :generate_centroids => [:make] do
  sh "itk_build/GenerateCentroids " +
     "Rat24"
end

desc "Extract primary Eigenvector to lon file"
task :extract_vectors => [:make] do
  sh "itk_build/ExtractVectors " +
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