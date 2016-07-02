# Usage: extract_opengl.rb <out> [in]...

outfile = ARGV.shift
defs = []

ARGV.each do |f|
  buf = File.read(f)
  buf.scan(/a?w?gl[A-Z][a-zA-Z0-9]*/) do |k|
    procname = "PFN" + k.upcase + "PROC"
    if buf.include? procname
      defs << k unless defs.include?(k)
    end
  end
end

defs.sort!

File.write(outfile, defs.join("\n"))

