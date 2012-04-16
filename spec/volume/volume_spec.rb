describe Sleuthkit::Volume do
  before :all do
		@sample_image = "spec/samples/test.image"
		puts "File #{@sample_image} not found!!" unless File.exist?(@sample_image)
		@image = Sleuthkit::Image.new(@sample_image)
		@string = "some string"
	end

	describe "#new(@disk_image)" do
		it "initializes with Sleuthkit::Image passed as param1" do
			@volume = Sleuthkit::Volume.new(@image)
			@volume.should be_an_instance_of Sleuthkit::Volume
		end
	end
	describe "#new(some_string)" do
		it "initializes and raises exception when initialized with an object of wrong class" do
			lambda { @volume = Sleuthkit::Volume.new(@string) }.should raise_error(TypeError)
		#	@volume.should raise_error( TypeError )
		end
	end
end


