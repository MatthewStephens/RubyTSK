describe "spec/image" do
  require 'sleuthkit'
  before :all do
    @sample_dir="samples"
    
    @orig_stderr = $stderr
    $stderr = StringIO.new
      
		@sample_filename = "#{@sample_dir}/tsk4r_img_01.dmg"
		@split_image_files = Dir.glob("#{@sample_dir}/tsk4r_img_01.dmg.split.?")
		
		puts "File #{@sample_image} not found!!" unless File.exist?(@sample_filename)
	end

	describe "#new(@sample_image)" do
		it "initializes with #{@sample_image} passed as arg1" do
			@image = Sleuthkit::Image.new(@sample_filename)
			@image.should be_an_instance_of Sleuthkit::Image
			@image.auto_detect.should eq(true)
		end
	end
	describe "#new(@sample_image, 1)" do
		it "initializes with specified TSK_IMG_TYPE_ENUM value of 1" do
			@image = Sleuthkit::Image.new(@sample_filename, 1)
			@image.auto_detect.should eq(false)
		end
	end
	describe "#new(@sammple_image, 3)" do
	  it "should fail to open disk when flag set to 3" do
	    @image = Sleuthkit::Image.new(@sample_filename, 3)
	    @image.description.should be_nil
    end
  end
  describe "#new(@sammple_image, :type_flag => 1)" do
	  it "initializes with specified TSK_IMG_TYPE_ENUM value of 1" do
	    @image = Sleuthkit::Image.new(@sample_filename, :type_flag => 1)
	    @image.auto_detect.should eq(false)
	    @image.size.should eq(40992768)
    end
  end
  describe "#new(@sammple_image, {:type_flag => 1, :offset => 0})" do
	  it "initializes with specified TSK_IMG_TYPE_ENUM value of 1, with extra option" do
	    @image = Sleuthkit::Image.new(@sample_filename, {:type_flag => 1, :offset => 0})
	    @image.auto_detect.should eq(false)
	    @image.size.should eq(40992768)
    end
  end
  describe "#new( *arr )" do
    it "should open a split raw image from array of filenames" do
      puts @split_image_files
      @image = Sleuthkit::Image.new(@split_image_files)
      @image.size.should eq(40992768)
      @image.description.should match("plit raw file")
    end
  end
  # attribute test
  describe "#new(@sample_image)" do
		it "initializes with #{@sample_image} passed as arg1" do
			@image = Sleuthkit::Image.new(@sample_filename)
			@image.should be_an_instance_of Sleuthkit::Image
			@image.sector_size.should eq(512)
			@image.size.should eq(40992768)
		end
	end
  # other
  describe "#pure_ruby(num)" do
    it "should pass a number from C layer up to Ruby layer during init" do
      @image = Sleuthkit::Image.new(@sample_filename)
      @image.should be_an_instance_of Sleuthkit::Image
    end
  end
  describe "Image#get_image_type_name(num)" do
    it "should return a description of a TSK Image type name, given its number" do
      @flag = Sleuthkit::Image.get_image_type_name(4)
      @flag.should match("aff")
    end
  end
  describe "Image#get_image_type_description(num)" do
    it "should return a description of a TSK Image type description, given its number" do
      @flag = Sleuthkit::Image.get_image_type_description(4)
      @flag.should match("Advanced Forensic Format")
    end
  end
  describe "Image#type_print" do
    it "should return a string reporting the types supported" do
      find_this="aff (Advanced Forensic Format)"
      escaped=Regexp.escape(find_this)
      @report = Sleuthkit::Image.type_print
      @report.should match(escaped)
    end
  end
  
  after :all do
    $stderr = @orig_stderr
  end
end

