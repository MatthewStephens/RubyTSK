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
  end
  
  # use `icat mac_fs_only_image_path inum` to troubleshoot
  describe "File Open by inum (inum)" do
    it "should return a file object sought by inum" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 28
      @tsk_file = Sleuthkit::FileSystem::FileData.new(@filesystem, inum)
      ap @tsk_file.inspect_object
      @tsk_file.meta.addr.should eq(inum)
      @tsk_file.should be_an_instance_of Sleuthkit::FileSystem::FileData 
    end
  end
  describe "File Open by name (absolute_name)" do
    it "should return a file object sought by absolute path name" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      absolute_name="/Test_Root_Folder/sample.txt"
      @tsk_file = Sleuthkit::FileSystem::FileData.new(@filesystem, absolute_name)
      ap @tsk_file.inspect_object
      @tsk_file.name.name.to_s.should match(File.basename(absolute_name))
      @tsk_file.meta.addr.should eq(28)
      @tsk_file.name.meta_addr.should eq(28)
      @tsk_file.name.parent_addr.should eq(26)
      @tsk_file.name.name_size.should eq(128)
      @tsk_file.should be_an_instance_of Sleuthkit::FileSystem::FileData 
    end
  end
  
  # same objects sought, but using FileSystem.find methods
  describe "FileSystem::System#find_file_by_inum (inum)" do
    it "should return a file object sought by inum" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 28
      @tsk_file = @filesystem.find_file_by_inum( inum )
      ap @tsk_file.inspect_object
      @tsk_file.meta.addr.should eq(inum)
      @tsk_file.should be_an_instance_of Sleuthkit::FileSystem::FileData 
    end
  end
  describe "FileSystem::System #find_file_by_name (absolute_name)" do
    it "should return a file object sought by absolute path name" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      absolute_name="/Test_Root_Folder/sample.txt"
      @tsk_file = @filesystem.find_file_by_name( absolute_name )
      ap @tsk_file.inspect_object
      @tsk_file.name.name.to_s.should match(File.basename(absolute_name))
      @tsk_file.meta.addr.should eq(28)
      @tsk_file.name.meta_addr.should eq(28)
      @tsk_file.name.parent_addr.should eq(26)
      @tsk_file.name.name_size.should eq(128)
      @tsk_file.should be_an_instance_of Sleuthkit::FileSystem::FileData 
    end
  end
  describe "FileSystem::System#find_file (inum)" do
    it "should return a file object sought by inum" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 28
      @tsk_file = @filesystem.find_file( inum )
      ap @tsk_file.inspect_object
      @tsk_file.meta.addr.should eq(inum)
      @tsk_file.should be_an_instance_of Sleuthkit::FileSystem::FileData 
    end
  end
  describe "FileSystem::System#find_file (absolute_name)" do
    it "should return a file object sought by absolute path name" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      absolute_name="/Test_Root_Folder/sample.txt"
      @tsk_file = @filesystem.find_file( absolute_name )
      ap @tsk_file.inspect_object
      @tsk_file.name.name.to_s.should match(File.basename(absolute_name))
      @tsk_file.meta.addr.should eq(28)
      @tsk_file.name.meta_addr.should eq(28)
      @tsk_file.name.parent_addr.should eq(26)
      @tsk_file.name.name_size.should eq(128)
      @tsk_file.should be_an_instance_of Sleuthkit::FileSystem::FileData 
    end
  end
  describe "FileSystem::File#get_number_of_attributes" do
    it "should return the number of file system attribute objects for the file" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      @file = @filesystem.find_by_inum(28)
      @file.should be_an_instance_of Sleuthkit::FileSystem::FileData
      num = @file.get_number_of_attributes
      num.should eq(1)
    end
  end
  
end