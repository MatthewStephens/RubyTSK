describe "spec/filesystem" do
  require 'sleuthkit'
  
  before :all do
    @sample_dir="samples"
    
    @mac_fs_only_image_path = "#{@sample_dir}/tsk4r_img_02.dmg"
    @mac_partitioned_image_path = "#{@sample_dir}/tsk4r_img_01.dmg"
    @split_image_files = Dir.glob("#{@sample_dir}/tsk4r*split.?")
    @split_image = Sleuthkit::Image.new(@split_image_files)
      
    puts "File #{@mac_fs_only_image_path} not found!!" unless File.exist?(@mac_fs_only_image_path)
    puts "File #{@mac_partitioned_image_path} not found!!" unless File.exist?(@mac_partitioned_image_path)
    
    @mac_fs_only_image = Sleuthkit::Image.new(@mac_fs_only_image_path)
    @mac_partitioned_image = Sleuthkit::Image.new(@mac_partitioned_image_path)
    
    @volume = Sleuthkit::VolumeSystem.new(@mac_partitioned_image)
    @string = "some string"
  end
  # check opening routines
  # simple open
  describe "#new([single raw image])" do
    it "initializes with Sleuthkit::Image passed as param1" do
      @filesystem = Sleuthkit::FileSystem.new(@mac_fs_only_image)
      @filesystem.should be_an_instance_of Sleuthkit::FileSystem
      @filesystem.description.should eq "hfs"
    end
  end
  describe "#new([split raw image])" do
    it "initializes with a Sleuthkit::VolumeSystem from split image passed as param1" do
      @filesystem = Sleuthkit::FileSystem.new(Sleuthkit::VolumeSystem.new(@split_image))
      @filesystem.should be_an_instance_of Sleuthkit::FileSystem
      @filesystem.description.should eq "hfs"
    end
  end
  describe "#new([image w/o filesystem])" do
    it "initializes with a split Sleuthkit::Image passed as param1" do
      @filesystem = Sleuthkit::FileSystem.new(@mac_partitioned_image)
      @filesystem.should be_an_instance_of Sleuthkit::FileSystem
      @filesystem.instance_variables.should eq []
    end
  end
  describe "new#([volume_system])" do
    it "initializes with Sleuthkit::VolumeSystem passed as param1" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.description.should eq "hfs"
    end
  end
  describe "new#([volume_partition])" do
    it "initializes with Sleuthkit::VolumePart passed as param1" do
      @partition = @volume.parts[3]
      @filesystem = Sleuthkit::FileSystem.new(@partition)
      @filesystem.description.should eq "hfs"
    end
  end
  describe "#new([str])" do
    it "initializes and raises exception when initialized with an object of wrong class" do
      lambda { @filesystem = Sleuthkit::FileSystem.new(@string) }.should raise_error(TypeError)
            #  @filesystem.should raise_error( TypeError )
    end
  end
  # open with :type_flag => TSK_FS_TYPE_ENUM (testing on 0 for now)
  describe "#new([single raw image], opts)" do
    it "initializes with Sleuthkit::Image passed as param1, with opts" do
      opts = { :type_flag => 0 }
      @filesystem = Sleuthkit::FileSystem.new(@mac_fs_only_image, opts)
      @filesystem.should be_an_instance_of Sleuthkit::FileSystem
      @filesystem.description.should eq "hfs"
    end
  end
  describe "new#([volume_system], opts)" do
    it "initializes with Sleuthkit::VolumeSystem passed as param1, with opts" do
      opts = { :type_flag => 0 }
      @filesystem = Sleuthkit::FileSystem.new(@volume, opts)
      @filesystem.description.should eq "hfs"
    end
  end
  describe "new#([volume_partition], opts)" do
    it "initializes with Sleuthkit::VolumePart passed as param1, with opts" do
      opts = { :type_flag => 0 }
      @partition = @volume.parts[3]
      @filesystem = Sleuthkit::FileSystem.new(@partition, opts)
      @filesystem.description.should eq "hfs"
    end
  end
  
  # check attribute readers for filesystem metadata
  
  describe "#block_count" do
    it "returns the @block_count attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.block_count.should eq(5004)
    end
  end
  describe "#block_post_size" do
    it "returns the @block_post_size attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.block_post_size.should eq(0)
    end
  end
  describe "#block_pre_size" do
    it "returns the @block_pre_size attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.block_pre_size.should eq(0)
    end
  end
  describe "#block_size" do
    it "returns the @block_size attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.block_size.should eq(4096)
    end
  end
  describe "#dev_bsize" do
    it "returns the @dev_bsize attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.dev_bsize.should eq(4096)
    end
  end  
  describe "#data_unit_name" do
    it "prints out the filesystem data unit name as a string" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.data_unit_name.should eq("Allocation Block")
    end
  end
  describe "#endian" do
    it "returns the @endian attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.endian.should eq(2)
    end
  end
  describe "#first_block" do
    it "returns the @first_block attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.first_block.should eq(0)
    end
  end
  describe "#first_inum" do
    it "returns the @first_inum attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.first_inum.should eq(2)
    end
  end
  describe "#flags" do
    it "returns the @flags attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.flags.should eq(0)
    end
  end
  describe "#fs_id" do
    it "returns the @fs_id attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.fs_id.should eq("ff83fbdcb863d7d8")
    end
  end
  describe "#fs_id_used" do
    it "returns the @fs_id_used attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.fs_id_used.should eq(16)
    end
  end
  describe "#ftype" do
    it "returns the @ftype attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.ftype.should eq(4096)
    end
  end
  describe "#inum_count" do
    it "returns the @inum_count attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.inum_count.should eq(45)
    end
  end
  describe "#isOrphanHunting" do
    it "returns the @isOrphanHunting attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.isOrphanHunting.should eq(0)
    end
  end
  describe "#journ_inum" do
    it "returns the @journ_inum attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.journ_inum.should eq(0)
    end
  end
  describe "#last_block" do
    it "returns the @last_block attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.last_block.should eq(5003)
    end
  end
  describe "#last_block_act" do
    it "returns the @last_block_act attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.last_block_act.should eq(5003)
    end
  end
  describe "#last_inum" do
    it "returns the @last_inum attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.last_inum.should eq(44)
    end
  end
  describe "#offset" do
    it "returns the @offset attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.offset.should eq(32768)
    end
  end
  # TO DO: "orphan_dir"
  describe "#root_inum" do
    it "returns the @root_inum attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.root_inum.should eq(2)
    end
  end
  describe "#tag" do
    it "returns the @tag attr" do
      @filesystem = Sleuthkit::FileSystem.new(@volume)
      @filesystem.tag.should eq(269488144)
    end
  end
    

  # other attributes
  describe "#get_filesystem_type" do
    it "prints out the filesystem type as a string" do
      @filesystem = Sleuthkit::FileSystem.new(@mac_fs_only_image)
      @filesystem.system_name.should eq("hfs")
    end
  end

  describe "#parent" do
    it "ensures the FS has a reference to the object it came from" do
      @filesystem = Sleuthkit::FileSystem.new(@mac_fs_only_image)
      @filesystem.parent.should eq(@mac_fs_only_image)
    end
  end
end


