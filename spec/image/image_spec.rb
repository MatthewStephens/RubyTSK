describe Sleuthkit::Image do
  before :all do
		@sample_image = "spec/samples/test.image"
		puts "File #{@sample_image} not found!!" unless File.exist?(@sample_image)
	end

	describe "#new" do
		it "initializes without parameters" do
			@image = Sleuthkit::Image.new
			@image.should be_an_instance_of Sleuthkit::Image
		end
	end
	describe "#new(@sample_image)" do
		it "initializes with #{@sample_image} passed as arg1" do
			@image = Sleuthkit::Image.new(@sample_image)
			@image.should be_an_instance_of Sleuthkit::Image
		end
	end
end

