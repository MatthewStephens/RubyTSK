# Rake tasks to prepare sample data objects for testing

namespace :spec do
  namespace :samples do
		SAMPLE_DIR="spec/samples"

    desc "Compress sample disk images"
    task :compress do
      require 'zlib'
      files = Dir.glob("spec/samples/*")
      puts files.join("\n")

      name = "test.image"
			fqname = "#{SAMPLE_DIR}/#{name}"
			
			# TO DO: offer some integrity checks
			checksum = String.new
			File.open("spec/samples/#{name}.md5") {|f|
				while (line = f.gets)
					checksum << line
				end
		 	}
			puts "checksum for #{name} is #{checksum}"	

			if File.exist?("#{SAMPLE_DIR}/#{name}")
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

    desc "Uncompress sample disk images"
    task :uncompress do
      require 'zlib'
      name = "test.image.gz"
			fqname = "#{SAMPLE_DIR}/#{name}"

			if File.exist?("#{SAMPLE_DIR}/#{name}")
        compressed = File.open("#{fqname}", 'rb')
        uncompressed = File.open("#{fqname.gsub(/.gz$/, '')}", 'wb')
  
        z=Zlib::GzipReader.new(compressed)
        
        uncompressed << z.read
        uncompressed.close
				File.delete("#{fqname}")
			else
				raise("could not locate #{fqname}.  Perhaps its already unzipped?")
			end
    end

    desc "splits a sample for use in tests"
    task :split do
			# dd if=spec/samples/tsk4r_img_01.dmg | split -b 10m - tsk4r_img_01.split.
			filename = "tsk4r_img_01.dmg"
			MEGABYTE = 1024 * 1024
			file = File.open("#{SAMPLE_DIR}/#{filename}", 'rb')
			parts = 0..3
			parts.each do |part|
				out = File.open("#{filename}.split.#{part}", 'wb')
				str = file.sysread(MEGABYTE * 10)
				out.syswrite(str)
				out.close
			end

    end

  end
end
