# -*- coding: utf-8 -*-
describe "spec/filesystem" do
  require 'sleuthkit'
  require 'awesome_print'
  
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
    @volume = Sleuthkit::Volume::System.new(@mac_partitioned_image)
    
    @ntfs_image_path = "#{@sample_dir}/tsk4r_img_ntfs.dmg"
    @ntfs_image = Sleuthkit::Image.new(@ntfs_image_path)
    @ntfs_vol = Sleuthkit::Volume::System.new(@ntfs_image)
    @ntfs = Sleuthkit::FileSystem::System.new(@ntfs_vol)
  end
  
  describe "Sleuthkit::FileSystem::Attribute.new(file)" do
    it "should return a FileSystem::Attribute object containing attributes (data) about a file" do
      size  = 42
      @addr = 28
      @fs = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      @file = @fs.find_file_by_inum(@addr)
      @file.should be_an_instance_of Sleuthkit::FileSystem::FileData
      
      @attr = Sleuthkit::FileSystem::Attribute.new(@file)
      @attr.file.should eq(@file)
      @attr.size.should eq(size)
    end
  end
  describe "Sleuthkit::FileSystem::Attribute.new(file, attribute_index)" do
    it "should return a FileSystem::Attribute object number {attribute_index} containing attributes (data) about a file" do
      size  = 42
      @addr = 28; index = 0
      @fs = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      @file = @fs.find_file_by_inum(@addr)
      @file.should be_an_instance_of Sleuthkit::FileSystem::FileData
      
      @attr = Sleuthkit::FileSystem::Attribute.new(@file, index)
      @attr.file.should eq(@file)
      @attr.size.should eq(size)
    end
  end
  describe "Sleuthkit::FileSystem::FileData#return_file_attributes" do
    it "should return one or more FileSystem::Attribute objects containing attributes (data) about a file" do
      size  = 42
      @addr = 28
      @fs = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      @file = @fs.find_file_by_inum(@addr)
      @file.should be_an_instance_of Sleuthkit::FileSystem::FileData
      
      @attr = @file.return_file_attributes
      @attr.file.should eq(@file)
      @attr.size.should eq(size)
    end
  end
  # use NTFS image for this one (other fs typically only have 1 attr)
  describe "File#get_number_of_attributes" do
    it "should return an integer representing the number of attribute entries for the file" do
      @addr = 73
      @file = @ntfs.find_file_by_inum( @addr )
      @file.should be_an_instance_of Sleuthkit::FileSystem::FileData
      @file.get_number_of_attributes.should eq(4)
    end
  end
  describe "File#return_file_attributes" do
    it "should return an array of FileSystem::Attributes objects" do
      @addr = 73
      @file = @ntfs.find_file_by_inum( @addr )
      @file.should be_an_instance_of Sleuthkit::FileSystem::FileData
      @atts = @file.return_file_attributes
      @atts.length.should eq(4)
      @atts[3].name.should eq("$I30")
      @atts[3].name_size.should eq(128)
      @atts[3].type.should eq(144)
      @atts[1].file.should eq @file
    end
  end
  
end