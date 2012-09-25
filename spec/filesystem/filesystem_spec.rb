describe Sleuthkit::FileSystem do
    before :each do
    @linux_image_path = "spec/samples/linux.iso"
    @mac_image_path = "spec/samples/tsk4r_img_01.dmg"
    puts "File #{@linux_image_path} not found!!" unless File.exist?(@linux_image_path)
    puts "File #{@mac_image_path} not found!!" unless File.exist?(@mac_image_path)
    
    @linux_image = Sleuthkit::Image.new(@linux_image_path)
    @mac_image = Sleuthkit::Image.new(@mac_image_path)
    
    #@volume = Sleuthkit::Volume.new(@image)
    @string = "some string"
  end
    
  describe "#new(@disk_image)" do
    it "initializes with Sleuthkit::Image passed as param1" do
      @filesystem = Sleuthkit::FileSystem.new(@linux_image)
      @filesystem.should be_an_instance_of Sleuthkit::FileSystem
      @filesystem.description.should eq "iso9660"
    end
  end
  describe "#(@volume_system)" do
    it "initializes with Sleuthkit::VolumeSystem passed as param1" do
      @volume = Sleuthkit::VolumeSystem.new(@mac_image)
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.description.should eq "hfs"
    end
  end
  describe "#(@partition)" do
    it "initializes with Sleuthkit::VolumePart passed as param1" do
      @volume = Sleuthkit::VolumeSystem.new(@mac_image)
      @partition = @volume.parts[3]
      @filesystem = Sleuthkit::FileSystem.new(@partition)
      @filesystem.description.should eq "hfs"
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
      @filesystem = Sleuthkit::FileSystem.new(@linux_image)
      @filesystem.block_size.should eq(2048)
    end
  end
    
  describe "#endian" do
    it "returns the @endian attr" do
      @filesystem = Sleuthkit::FileSystem.new(@linux_image)
      @filesystem.endian.should eq(2)
    end
  end
    
  describe "#offset" do
    it "returns the @offset attr" do
      @filesystem = Sleuthkit::FileSystem.new(@linux_image)
      @filesystem.offset.should eq(0)
    end
  end
    
  describe "#inum_count" do
    it "returns the @inum_count attr" do
      @filesystem = Sleuthkit::FileSystem.new(@linux_image)
      @filesystem.inum_count.should eq(4)
    end
  end
  
  describe "#get_filesystem_type" do
    it "prints out the filesystem type as a string" do
      @filesystem = Sleuthkit::FileSystem.new(@linux_image)
      @filesystem.system_name.should eq("iso9660")
    end
  end
    
end


