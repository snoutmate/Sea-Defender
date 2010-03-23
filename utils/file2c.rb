#!/usr/bin/env ruby

raise "fname fnameout [type]" if (ARGV.count<2)

$fname = ARGV[0]
$fnameout = ARGV[1]
$type = ARGV[2] || "binary"

$sane_fname = $fname.gsub(/[^a-zA-Z]/, '_')

def convert_binary(f,fout)
	fout << "\n"
	fout << "const unsigned char #{$sane_fname}[] = {\n\t"
	count = 0
	f.each_byte do |byte|
		fout << "0x"
		fout << "0" if byte < 16
		fout << "#{byte.to_s(16)},"
		count += 1
		fout << "\n\t" if count % 16 == 0
	end
	fout << "\n};\n\n"
end

def convert_text(f,fout)
	fout << "\n"
	fout << "const char #{$sane_fname}[] = \"\"\n"
	f.each_line do |line|
		line.chomp!
#		fout << "0" if byte < 16
#		fout << "#{byte.to_s(16)},"
		fout << "\"" << line << "\\n\"\n";
	end
	fout << ";\n\n"
end

File.open($fname,"r") do |f|
	File.open($fnameout,"w") do |fout|
		if ($type=="text"||$type=="txt")
			convert_text(f,fout)
		else
			convert_binary(f,fout)
		end
	end
end
