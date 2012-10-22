describe "spec/image" do
  require 'sleuthkit'
  before :all do
    @sample_dir="samples"
    
    @orig_stderr = $stderr
    $stderr = StringIO.new
      
    @sample_filename = "#{@sample_dir}/tsk4r_img_01.dmg"
    @sample2_filename = "#{@sample_dir}/tsk4r_img_02.dmg"
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
      @image.description.should eq("Split raw files")
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
      find_this="raw (Single raw file (dd))"
      escaped=Regexp.escape(find_this)
      @report = Sleuthkit::Image.type_print
      @report.should match(escaped)
    end
  end
  # read methods
  describe "Image#read_block" do
    it "Reads {num} blocks starting at a block address {addr} relative to the start of an Image" do
      @image = Sleuthkit::Image.new(@sample2_filename)
      sector_addr = 28656; num_blocks = 1
      find_this = "this is a test txt file.\nIt has two lines."
      @blocks = @image.read_block(sector_addr, num_blocks)
      @blocks.should match(find_this)
    end
  end
  describe "Image#read_block" do
    it "Reads {num} blocks starting at a block address {addr} relative to the start of an Image" do
      @image = Sleuthkit::Image.new(@sample_filename)
      sector_addr = 1; num_blocks = 1
      find_this = /Apple\x00{27}Apple_partition_map/
      @blocks = @image.read_block(sector_addr, num_blocks)
      @blocks.should match(find_this)
    end
  end
  describe "Image#read_bytes" do
    it "Reads {num} blocks starting at a block address {addr} relative to the start of an Image" do
      @image = Sleuthkit::Image.new(@sample_filename)
      byte_addr = 512; num_bytes = 256
      find_this = /Apple\x00{27}Apple_partition_map/
      @blocks = @image.read_bytes(byte_addr, num_bytes)
      @blocks.should match(find_this)
    end
  end
  
  after :all do
    $stderr = @orig_stderr
  end
end

