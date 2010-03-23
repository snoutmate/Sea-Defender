#!/usr/bin/env ruby
# 
# converts SVG font to .mfont
#
# to create SVG font from TTF, you can use ttf2svg from batik package (libbatik-java on Ubuntu)
#
range_start = 32 # ASCII 32-127 (all printable chars)
range_end = 127

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

  horiz_advance = 0
  em_scale = 0
  ascent = 0
  descent = 0

  doc.elements.each("svg/defs/font") { |element|
    horiz_advance = (element.attributes["horiz-adv-x"]).to_i
  }

  doc.elements.each("svg/defs/font/font-face") { |element|
    em_scale = (element.attributes["units-per-em"]).to_i
    ascent = (element.attributes["ascent"]).to_i
    descent = (element.attributes["descent"]).to_i
  }

  chars = {}
  (range_start..range_end).each {|i| chars[i] = [0,[]]}

  allnums = []

  doc.elements.each("svg/defs/font/glyph") { |element|
    n = element.attributes["unicode"]
    advance = element.attributes["horiz-adv-x"]
    path = element.attributes["d"] || ""

    # SVG spec allows for whitespace(and commas) omission in cases where it's considered superfluous,
    # so fill the spaces back in to simplify parsing
    path.gsub!(","," ")
    
    # 2 passes for ABA -> A B A matching
    2.times do 
      path.gsub!(/([a-zA-Z](\d|\-))|(\d[a-zA-Z])|([a-zA-Z][a-zA-Z])/) {|m| m[0].chr+" "+m[1].chr }
    end

    chars[n[0].ord] = [advance,path.split]

    allnums << advance.to_i

    # store values for normalization    
    path.split.each do |p|
      next if p =~ /[a-zA-Z]/
      allnums << p.to_i
    end
  }

  # hkern

  name_to_unicode = {}
  doc.elements.each("svg/defs/font/glyph") { |element|
    uni = element.attributes["unicode"]
    name = element.attributes["glyph-name"]
    name_to_unicode[name] = uni
  } 

  hkern = []

  doc.elements.each("svg/defs/font/hkern") { |element|
    g1 = element.attributes["g1"]
    g2 = element.attributes["g2"]
    k = element.attributes["k"]
    uni1 = name_to_unicode[g1]
    uni2 = name_to_unicode[g2]
    next if uni1.nil? || uni2.nil?
    hkern << [uni1,uni2,k.to_f]
  }

#  p hkern.size


  # normalize
  min = allnums.min
  max = allnums.max
  @adder = min.to_f
  puts max
  puts min
  @divisor = 1/65535.0 * (max - min).to_f

  # output
  fo << [(horiz_advance/@divisor).round,
         (em_scale/@divisor).round,
         (ascent/@divisor).round,
         (@adder/@divisor).abs.round].pack("S*")

  (range_start..range_end).each  do |i|
    char = chars[i]
    fo << [(char.first.to_f / @divisor).round].pack("S")
    (char.last).each do |pe|
      if pe =~ /[a-zA-Z]/
        fo << pe
      else
        nor = normalize(pe.to_f)
        fo << [(nor).round].pack("S")
      end
    end
    fo << "\n"
  end

#  hkern.each do |hk|
#    fo << hk[0] << hk[1] << [(hk[2]/@divisor).round].pack("S")
#  end
}

