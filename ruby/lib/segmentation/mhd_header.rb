module Segmentation
  class MhdHeader
    def initialize(path)
      @lines = File.readlines path
    end
    
    def element_spacing
      line = @lines.select {|l| l =~ /^ElementSpacing/}[0]
      line.split[-3..-1].map &:to_f
    end
    
    def dim_size
      line = @lines.select {|l| l =~ /^DimSize/}[0]
      line.split[-3..-1].map &:to_i
    end
  end
end