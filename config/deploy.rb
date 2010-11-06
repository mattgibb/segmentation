set :application, "segmentation"
set :repository, "git://github.com/mattgibb/segmentation.git"
set :scm, :git
set :deploy_via, :copy
set :copy_cache, true
set :deploy_to, "~/imaging/#{application}"
set :use_sudo, false

role :servers, "heart"
# role :clpcs, "work"

# set :local_dir, "/Users/matthewgibb/Code/imaging/registration"

desc "Compile all the itk programs on the servers"
task :compile do
  run "cd #{deploy_to}/current/itk_build; make"
end
