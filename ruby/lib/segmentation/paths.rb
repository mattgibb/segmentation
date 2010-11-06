module Segmentation
  module Paths
    PROJECT_ROOT_DIR = File.expand_path(File.join(File.dirname(__FILE__), "../../../")) + "/"
    IMAGES_DIR = File.join(PROJECT_ROOT_DIR, "images/")
    CONFIG_DIR = File.join(PROJECT_ROOT_DIR, "config/")
    RESULTS_DIR = File.join(PROJECT_ROOT_DIR, "results/")
    ITK_DIR = File.join(PROJECT_ROOT_DIR, "itk_build/")
    
    def itk_program(name)
      File.join(ITK_DIR, name)
    end
    
    def dtmri_dir(data_set)
      File.join IMAGES_DIR, data_set, 'MRI/DTMRI/'
    end
    
    def results_dir(data_set)
      File.join RESULTS_DIR, data_set, 'segmentation/'
    end
    
    def vox_path(data_set)
      File.join results_dir(data_set), "voxels.vox"
    end
  end
end
