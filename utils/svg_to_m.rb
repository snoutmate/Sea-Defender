#!/usr/bin/env ruby

# converts SVG paths to .m
#
# .m format is equivalent to SVG path element, with no whitespace, commands 'a-zA-Z' unchanged,
#  and each number normalized into 0.0-1.0 range and converted into unsigned short (0-65535)

require 'rexml/document'
include REXML

require 'vector.rb'

#ARGV = ["../data/model/kronika.svg","../data/model/kronika.mfont"]
if ARGV.count <2
  puts "Usage: #{$0} infile.svg outfile.mfont"
  exit
end


def normalize(num)
  num -= @adder
  num /= @divisor
  num
end

infile,outfile,*other = ARGV

fo = File.open(outfile,"w")

File.open(infile,"r") {|f|
  doc = Document.new(f)

  paths = []
  allnums = []

  ["svg/g/path","svg/path"].each do |ename|
    doc.elements.each(ename) { |element|
      path = element.attributes["d"] || ""
    
      # SVG spec allows for whitespace(and commas) omission in cases where it's considered superfluous,
      # so fill the spaces back in to simplify parsing
      path.gsub!(","," ")
      
      # 2 passes for ABA -> A B A matching
      2.times do 
        path.gsub!(/([a-zA-Z](\d|\-))|(\d[a-zA-Z])|([a-zA-Z][a-zA-Z])/) {|m| m[0].chr+" "+m[1].chr }
      end
  
      path = path.split
  
      # store values for normalization    
      path.each do |p|
        next if p =~ /[a-zA-Z]/
        allnums << p.to_f
      end
  
      paths << path
    }
  end


  # normalize
  min = allnums.min
  max = allnums.max
  @adder = min.to_f
  @divisor = 1/65535.0 * (max - min).to_f

  paths.each do |path|
    (path).each do |pe|
      if pe =~ /[a-zA-Z]/
        fo << pe
      else
        nor = normalize(pe.to_f).round
        #	puts nor
        fo << [nor].pack("S")
      end
    end
  end
}

