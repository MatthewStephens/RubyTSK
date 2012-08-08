describe Sleuthkit::FileSystem do
    before :each do
    @sample_image = "spec/samples/linux.iso"
    puts "File #{@sample_image} not found!!" unless File.exist?(@sample_image)
    @image = Sleuthkit::Image.new(@sample_image)
    #@volume = Sleuthkit::Volume.new(@image)
    @string = "some string"
  end
    
  describe "#new(@disk_image)" do
    it "initializes with Sleuthkit::Image passed as param1" do
      @filesystem = Sleuthkit::FileSystem.new(@image)
      @filesystem.should be_an_instance_of Sleuthkit::FileSystem
    end
  end
  describe "#new(some_string)" do
    it "initializes and raises exception when initialized with an object of wrong class" do
      lambda { @filesystem = Sleuthkit::FileSystem.new(@string) }.should raise_error(TypeError)
            #  @filesystem.should raise_error( TypeError )
    end
  end
    
  # check attribute readers for filesystem metadata
  describe "#block_size" do
    it "returns the @block_size attr" do
      @filesystem = Sleuthkit::FileSystem.new(@image)
      @filesystem.block_size.should eq(2048)
    end
  end
    
  describe "#endian" do
    it "returns the @endian attr" do
      @filesystem = Sleuthkit::FileSystem.new(@image)
      @filesystem.endian.should eq(2)
    end
  end
    
  describe "#offset" do
    it "returns the @offset attr" do
      @filesystem = Sleuthkit::FileSystem.new(@image)
      @filesystem.offset.should eq(0)
    end
  end
    
  describe "#inum_count" do
    it "returns the @inum_count attr" do
      @filesystem = Sleuthkit::FileSystem.new(@image)
      @filesystem.inum_count.should eq(4)
    end
  end
    
end


