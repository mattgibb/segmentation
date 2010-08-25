module Segmentation
  class MeshData
    attr_accessor :points, :elements, :regions
    
    def centroids
      print "Calculating centroids..."
      @centroids ||= elements.map do |element|
        centroid(element)
      end
      puts "done"
    end
    
    def centroid(element)
      # retrieve coordinates from point indeces
      tetra_coords = element.map do |point|
        points[point]
      end
      
      # average the four xs, ys and zs
      tetra_coords.transpose.map do |coord|
        coord.inject(&:+) / 4
      end
      
    end
  end
  
  class CarpReader
    attr_reader :mesh_data
    def initialize(data_path)
      @pts_file  = "#{data_path}.pts"
      @elem_file = "#{data_path}.elem"
      @mesh_data = MeshData.new
      
      extract_points
      extract_elements
      extract_regions
    end
    
    def extract_points
      print "Extracting points..."
      @mesh_data.points = pts_lines.map do |line|
        line.split.map &:to_f
      end
      puts "done"
    end
    
    def extract_elements
      print "Extracting elements..."
      @mesh_data.elements = read_elem_file(1..4)
      puts "done"
    end
    
    def extract_regions
      print "Extracting regions..."
      @mesh_data.regions = read_elem_file(5..5)
      puts "done"
    end
    
    def read_elem_file(range)
      elem_lines.map do |line|
        line.split[range].map &:to_i
      end
    end
    
    def pts_lines
      @pts_lines ||= File.readlines(@pts_file)[1..-1]
    end
    
    def elem_lines
      @elem_lines ||= File.readlines(@elem_file)[1..-1]
    end
    
  end
end

