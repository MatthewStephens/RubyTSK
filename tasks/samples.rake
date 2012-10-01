# Rake tasks to prepare sample data objects for testing

namespace :spec do
  namespace :samples do
    SAMPLE_DIR="samples"
    MEGABYTE = 1024 * 1024

    desc "Globs list of disks to zip/unzip"
    task :glob, :gz do |t, args|
      suffix = args[:gz] || ''
      puts "suffix: #{suffix}"
      arr = [ "#{SAMPLE_DIR}/*.dmg", "#{SAMPLE_DIR}/*.iso", "#{SAMPLE_DIR}/*.image" ]
      arr.each {|a| a << suffix }
     puts arr.inspect 
      @glob=Array.new
      proc=Proc.new { |str| Dir.glob(str) }
      arr.each {|e| @glob << proc.call(e) }
      @glob.flatten!
      puts @glob
    end
       
    desc "Compress sample disk images"
    task :compress, [:file] do |t, args|
      require 'zlib'
      Rake::Task["spec:samples:glob"].invoke     
      args[:file].nil? ? files = @glob : files = args[:file].to_a 

      files.each {|e| e.gsub!(/\.?\/?#{SAMPLE_DIR}\//, '') }
      puts "compressing " + files.join("\n")

      files.each do |name|
        fqname = "#{SAMPLE_DIR}/#{name}"
        
        puts "fqname: #{fqname}"
        if File.exist?("#{fqname}")
          uncompressed = File.open("#{fqname}", 'rb')
    
          Zlib::GzipWriter.open("#{fqname}.gz") do |gz|
            while (line = uncompressed.gets ) 
              gz.write line
            end
          end
          uncompressed.close
          File.delete("#{fqname}")
  
        else
          raise("could not locate #{fqname}.  Perhaps it's already zipped?")
        end
      end

    end

    desc "Uncompress sample disk images"
    task :uncompress, [:file]  do |t, args|
      require 'digest/md5'
      require 'zlib'

      Rake::Task["spec:samples:glob"].invoke('.gz')     
      args[:file].nil? ? files = @glob : files = args[:file].to_a 

      files.each {|e| e.gsub!(/\.?\/?#{SAMPLE_DIR}\//, '') }

      files.each do |name|
        fqname = "#{SAMPLE_DIR}/#{name}"
        new_name = fqname.gsub(/\.gz$/, '')
        puts "uncompressing #{fqname}"
  
        if File.exist?("#{fqname}")
          compressed = File.open("#{fqname}", 'rb')
          uncompressed = File.open("#{new_name}", 'wb')
    
          z=Zlib::GzipReader.new(compressed)
          
          uncompressed << z.read
          uncompressed.close
          File.delete("#{fqname}")
        else
          raise("could not locate #{fqname}.  Perhaps its already unzipped?")
        end
        # finish with some integrity checking
        verify("#{fqname.gsub(/\.gz/, '')}")
      end

    end

    desc "splits a sample for use in tests"
    task :split, [:file] do |t, args|
      Rake::Task["spec:samples:glob"].invoke     
      args[:file].nil? ? files = @glob : files = args[:file].to_a 

      # to do something similar on the command line:
      # dd if=spec/samples/tsk4r_img_01.dmg | split -b 10m - tsk4r_img_01.split.
      
      files.each {|e| e.gsub!(/\.?\/?#{SAMPLE_DIR}\//, '') }
      files.each do |filename|
				fqname = "#{SAMPLE_DIR}/#{filename}"
				puts "reading #{fqname}"
        file = File.open("#{fqname}", 'rb')
				num_parts = ( File.size?(fqname) / (MEGABYTE * 10) ) + 1
				parts = Array.new(num_parts)
        parts.each_index do |index|
          out = File.open("#{fqname}.split.#{index}", 'wb')
					puts "reading #{MEGABYTE * 10} bytes"
          str = file.sysread(MEGABYTE * 10)
          out.syswrite(str)
          out.close
        end
      end
    end
    
    def verify(filename)
      require 'digest/md5'
        checksum_report = String.new
        digest = Digest::MD5.hexdigest(File.read(filename))
        File.open("#{SAMPLE_DIR}/keys/#{File.basename(filename)}.md5") {|f|
          while (line = f.gets)
            checksum_report << line
          end
         }
        expected = checksum_report.match(/#{digest}/)
        if expected.to_s == digest
          puts "#{filename} checksum verified"
        else
          puts "checksum_report for #{filename} is #{digest}, expected #{checksum_report}"  
        end
    end

  end
end
