# Rake tasks to prepare sample data objects for testing

namespace :spec do

  # defining a default task in the spec:samples namespace
	desc "setup material for testing"
  task :samples do
	  Rake::Task['spec:samples:check'].execute
	  Rake::Task['spec:samples:uncompress'].execute
	  Rake::Task['spec:samples:split'].execute
  end

  namespace :samples do
    SAMPLE_DIR="samples"
    MEGABYTE = 1024 * 1024
		HOME = File.expand_path("..", File.dirname("#{__FILE__}"))

    desc "Get samples into shape for testing"
		task :check do
			Dir.chdir("#{HOME}")
			status = %x[ git submodule status ]
			if status.match("#{SAMPLE_DIR} (heads/master)")
				# submodule initiated already, check if files in place
				Rake::Task['spec:samples:verify_files'].invoke
			elsif status.match("#{SAMPLE_DIR}$")
				# submodule need update --init
				Rake::Task['spec:samples:init'].invoke
			elsif status.empty?
				# submodule is OK, check files in place
				Rake::Task['spec:samples:verify_files'].invoke
			else 
				# status ??
			end
		end
		
		desc "Go into samples and verify files are there"
		task :verify_files do
			Dir.chdir("#{HOME}/#{SAMPLE_DIR}")
			status = %x[ git status ]
			if status.match("deleted:")
				Rake::Task['spec:samples:revert'].invoke
			else
				puts "git reports no files deleted in #{SAMPLE_DIR}"
			end
		end

		desc "Downloads sample images from github"
		task :init do
			Dir.chdir(HOME)
		  system "git submodule update --init"
		end
		
		desc "restore the images to original state"
		task :revert do
			Dir.chdir("#{HOME}/#{SAMPLE_DIR}")
			reset = %x[ git reset --hard  ]
			puts reset
			puts "resetting #{Dir.pwd}"
		end

#    desc "Globs list of disks to zip/unzip"
    task :glob, :gz do |t, args|
      suffix = args[:gz] || ''
      puts "suffix: #{suffix}"
      arr = [ "#{SAMPLE_DIR}/*.dmg", "#{SAMPLE_DIR}/*.iso", "#{SAMPLE_DIR}/*.image" ]
      arr.each {|a| a << suffix }
      puts "  #{arr.inspect}" 
      @glob=Array.new
      proc=Proc.new { |str| Dir.glob(str) }
      arr.each {|e| @glob << proc.call(e) }
      @glob.flatten!
      puts "  #{@glob.inspect}"
			@glob
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

			puts "building file list"
      Rake::Task["spec:samples:glob"].invoke('.gz')     
			files = []
      args[:file].nil? ? files = @glob : files <<  args[:file] 

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
          raise("could not locate #{fqname}.  Perhaps it is already unzipped?")
        end
        # finish with some integrity checking
        verify("#{fqname.gsub(/\.gz/, '')}")
      end

    end

    desc "splits a sample for use in tests"
    task :split, [:file] do |t, args|
			puts "building file list"
      Rake::Task["spec:samples:glob"].execute     
			files = []
      args[:file].nil? ? files = @glob : files << args[:file] 
      # to do something similar on the command line:
      # dd if=spec/samples/tsk4r_img_01.dmg | split -b 10m - tsk4r_img_01.split.
      
      files.each {|e| e.gsub!(/\.?\/?#{SAMPLE_DIR}\//, '') }
      files.each do |filename|
				fqname = "#{SAMPLE_DIR}/#{filename}"
				puts "reading #{fqname}"
        file = File.open("#{fqname}", 'rb')
				puts "#{File.size?(fqname)} for size of #{fqname}"
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
