def gl_type(str)
  "PFN#{str.upcase}PROC"
end

functions_in = File.read ARGV.shift
header_in = File.read ARGV.shift
source_in = File.read ARGV.shift
functions = []

functions_in.each_line do |line|
  l = line.chomp
  unless l.empty? || l[0] == '#'
    functions << l
  end
end

header_defs = functions.map do |func|
  "extern #{gl_type func} #{func};"
end

source_defs = functions.map do |func|
  "#{gl_type func} #{func} = 0;"
end

source_body = "\nGLL_API void GLL_LoadAllFunctions(void)\n{\n"
functions.each do |func|
  source_body += "    #{func} = (#{gl_type func})GLL_LoadFunction(\"#{func}\");\n"
end
source_body += "}\n"

header_data = header_defs.join("\n")
source_data = source_defs.join("\n") + "\n" + source_body

File.write(ARGV.shift, header_in.sub('%%', header_data))
File.write(ARGV.shift, source_in.sub('%%', source_data))

