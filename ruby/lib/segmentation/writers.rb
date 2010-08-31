class CardiovizWriter
  def initialize(file, number_of_elements, title)
    @file = file
    @number_of_elements = number_of_elements
    @title = title
    @dimension = 3
  end
  
  def write_header
    @file.puts @title
    @file.puts 2 # association flag: 1 for points / 2 for cells
    @file.puts "#{@number_of_elements} #{@dimension}"
  end
  
  def write_element(element)
    @file.print element
  end
  
end

class LonWriter
  def initialize(file, number_of_elements)
    @file = file
    @number_of_elements = number_of_elements
  end
  
  def write_header
    @file.puts @number_of_elements
  end
  
  def write_element(element)
    @file.puts element.join(" ")
  end
  
end
