describe "spec/volume" do
  require 'sleuthkit'
  
  before :each do
    @sample_dir="samples"
    
		@mac_partitioned_image = "#{@sample_dir}/tsk4r_img_01.dmg"
		@split_image_files = Dir.glob("#{@sample_dir}/tsk4r_img_01.dmg.split*?")
		puts "File #{@mac_partitioned_image} not found!!" unless File.exist?(@mac_partitioned_image)
		@partitioned_image = Sleuthkit::Image.new(@mac_partitioned_image)
		@split_image = Sleuthkit::Image.new(@split_image_files) # split version of above
		@string = "some string"
	end

	describe "#new(@disk_image)" do
		it "initializes with Sleuthkit::Image passed as param1" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@volume.should be_an_instance_of Sleuthkit::Volume::System
		end
	end
	describe "#new(@disk_image, {:offset => 0} )" do
		it "returns Volume::System found in Sleuthkit::Image @disk_image at offset #idx" do
		  @opts = {:offset => 0} # TO DO
			@volume = Sleuthkit::Volume::System.new(@partitioned_image, @opts)
			@volume.offset.should eq(0)
		end
	end
	describe "#new(@disk_image, {:offset => 0, :type_flag => 0})" do
		it "returns Volume::System found in Sleuthkit::Image with 2 opts selected" do
		  @opts = {:offset => 0, :type_flag => 0}
			@volume = Sleuthkit::Volume::System.new(@partitioned_image, @opts)
			@volume.offset.should eq(0)
		end
	end
	describe "#new(some_string)" do
		it "initializes and raises exception when initialized with an object of wrong class" do
			lambda { @volume = Sleuthkit::Volume::System.new(@string) }.should raise_error(TypeError)
		#	@volume.should raise_error( TypeError )
		end
	end

	# check attribute readers for volume metadata
	describe "#block_size" do
		it "returns the @block_size attr" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@volume.block_size.should eq(512)
		end
	end

	describe "#endian" do
		it "returns the @endian attr" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@volume.endian.should eq(2)
		end
	end

	describe "#offset" do
		it "returns the @offset attr" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@volume.offset.should eq(0)
		end
	end

	describe "#partition_count" do
		it "returns the @partition_count attr" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@volume.partition_count.should eq(6)
		end
	end
  
	describe "#volume_system_type" do
		it "returns the @volume_system_type attr" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@volume.volume_system_type.should eq(8)
		end
	end
  
	describe "#description" do
		it "returns the @description attr" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@volume.description.should eq("MAC Partition Map")
		end
	end
  
  describe "#parent" do
    it "ensures the VS has a reference to the disk it came from" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
      @volume.parent.should eq(@partitioned_image)
    end
  end
	
	# extracting and examinging Volume::Partition objects
	describe "#[0]" do
		it "returns the first Volume::Partition object" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@part = @volume[0]
			@part.flags.should eq(2)
		end
	end
	describe "Volume::Partition#description" do
		it "returns the @description attr" do
			@volume = Sleuthkit::Volume::System.new(@partitioned_image)
			@part = @volume.parts.firstVolume::System
			@part.description.should eq("Unallocated")
		end
	end
	describe "Volume::Partition#parent" do
	  it "returns the ID of the parent Volume::System object" do
	    @volume = Sleuthkit::Volume::System.new(@partitioned_image)
	    @part = @volume.parts.first
	    @part.parent.should eq(@volume)
    end
  end
  describe "Volume::System#init from raw split images" do
    it "returns a Volume::System from a split disk image, built from an array" do
      @volume = Sleuthkit::Volume::System.new(@split_image)
      #pp @volume.inspect_object
      #pp @split_image.inspect_object
      @volume.parts.length.should eq(6)
      #pp "Tests ran with the following gems loaded"
      #pp Gem.loaded_specs.values.map {|x| "#{x.name} #{x.version}"}
    end
  end
  describe "#inspect_object" do
    it "returns the instance variables as a hash" do
      @volume = Sleuthkit::Volume::System.new(@partitioned_image)
      @volume.inspect_object.should be_an_instance_of(Hash)
      @volume.inspect_object[:description].should eq @volume.description
    end
  end
  
end


