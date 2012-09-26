describe Sleuthkit::Image do
  before :all do
    @orig_stderr = $stderr
    $stderr = StringIO.new
      
		@sample_image = "spec/samples/tsk4r_img_01.dmg"
		puts "File #{@sample_image} not found!!" unless File.exist?(@sample_image)
	end

	describe "#new(@sample_image)" do
		it "initializes with #{@sample_image} passed as arg1" do
			@image = Sleuthkit::Image.new(@sample_image)
			@image.should be_an_instance_of Sleuthkit::Image
		end
	end
	describe "#new(@sample_image, 1)" do
		it "initializes with specified TSK_IMG_TYPE_ENUM value of 1" do
			@image = Sleuthkit::Image.new(@sample_image, 1)
			@image.should be_an_instance_of Sleuthkit::Image
		end
	end
	describe "#new(@sammple_image, 3)" do
	  it "should fail to open disk when flag set to 3" do
	    @image = Sleuthkit::Image.new(@sample_image, 3)
	    #@image.instance_variables.should eq []
	    $stderr.rewind
      $stderr.string.chomp.should match("unable to open disk")
    end
  end
  
  after :all do
    $stderr = @orig_stderr
  end
end

