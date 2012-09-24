describe "spec/volume" do
  before :each do
		@sample_image = "spec/samples/tsk4r_img_01.dmg"
		puts "File #{@sample_image} not found!!" unless File.exist?(@sample_image)
		@image = Sleuthkit::Image.new(@sample_image)
		@string = "some string"
	end

	describe "#new(@disk_image)" do
		it "initializes with Sleuthkit::Image passed as param1" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@volume.should be_an_instance_of Sleuthkit::VolumeSystem
		end
	end
	describe "#new(some_string)" do
		it "initializes and raises exception when initialized with an object of wrong class" do
			lambda { @volume = Sleuthkit::VolumeSystem.new(@string) }.should raise_error(TypeError)
		#	@volume.should raise_error( TypeError )
		end
	end

	# check attribute readers for volume metadata
	describe "#block_size" do
		it "returns the @block_size attr" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@volume.block_size.should eq(512)
		end
	end

	describe "#endian" do
		it "returns the @endian attr" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@volume.endian.should eq(2)
		end
	end

	describe "#offset" do
		it "returns the @offset attr" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@volume.offset.should eq(0)
		end
	end

	describe "#partition_count" do
		it "returns the @partition_count attr" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@volume.partition_count.should eq(6)
		end
	end
  
	describe "#volume_system_type" do
		it "returns the @volume_system_type attr" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@volume.volume_system_type.should eq(8)
		end
	end
  
	describe "#description" do
		it "returns the @description attr" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@volume.description.should eq("MAC Partition Map")
		end
	end
	
	# extracting and examinging VolumePart objects
	describe "#expose_part" do
		it "returns a VolumePart object" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@part = @volume.expose_part
			@part.flags.should eq(2)
		end
	end
	describe "VolumePart#description" do
		it "returns the @description attr" do
			@volume = Sleuthkit::VolumeSystem.new(@image)
			@part = @volume.expose_part
			@part.description.should eq("Unallocated")
		end
	end
	describe "VolumePart#parent" do
	  it "returns the ID of the parent VolumeSystem object" do
	    @volume = Sleuthkit::VolumeSystem.new(@image)
	    @part = @volume.parts.first
	    @part.parent.should eq(@volume.object_id)
    end
  end
end


