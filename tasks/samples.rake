# Rake tasks to prepare sample data objects for testing

namespace :spec do
  namespace :samples do
    desc "Compress sample disk images"
    task :compress do
      require 'zlib'
      files = Dir.glob("spec/samples/*")
      puts files.join("\n")

      name = "test.image"
			checksum = String.new
			File.open("spec/samples/#{name}.md5") {|f|
				while (line = f.gets)
					checksum << line
				end
		 	}
			puts "checksum for #{name} is #{checksum}"	
      uncompressed = File.open("spec/samples/#{name}", 'r')

      Zlib::GzipWriter.open("#{name}.gz") do |gz|
        while (line = uncompressed.gets ) 
          gz.write line
        end
      end


    end

    desc "Uncompress sample disk images"
    task :uncompress do
      require 'zlib'
      name = "test.image.gz"
      compressed = File.open("spec/samples/#{name}", 'r')
      uncompressed = File.open("#{name.gsub(/.gz$/, '')}", 'w')

      z=Zlib::GzipReader.new(file)
      
      uncompressed << z.read
      uncompressed.close
    end

    desc "splits a sample for use in tests"
    task :split do
			# dd if=spec/samples/tsk4r_img_01.dmg | split -b 10m - tsk4r_img_01.split.
			SAMPLE_DIR="spec/samples"
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
