# -*- coding: utf-8 -*-
describe "spec/filesystem" do
  require 'sleuthkit'
  require 'awesome_print'
  
  before :all do
    @sample_dir="samples"
    
    @fat12_path ="/usr/local/forensics/samples/sample1.img"
    @mac_fs_only_image_path = "#{@sample_dir}/tsk4r_img_02.dmg"
    @mac_partitioned_image_path = "#{@sample_dir}/tsk4r_img_01.dmg"
    @split_image_files = Dir.glob("#{@sample_dir}/tsk4r*split.?")
    @split_image = Sleuthkit::Image.new(@split_image_files)
      
    puts "File #{@mac_fs_only_image_path} not found!!" unless File.exist?(@mac_fs_only_image_path)
    puts "File #{@mac_partitioned_image_path} not found!!" unless File.exist?(@mac_partitioned_image_path)
    
    @fat12_image = Sleuthkit::Image.new(@fat12_path)
    @mac_fs_only_image = Sleuthkit::Image.new(@mac_fs_only_image_path)
    @mac_partitioned_image = Sleuthkit::Image.new(@mac_partitioned_image_path)
    
    @volume = Sleuthkit::Volume::System.new(@mac_partitioned_image)
  end
  
  # use `icat mac_fs_only_image_path inum` to troubleshoot
  describe "Meta Open by inum (filesystem, inum)" do
    it "should return a file meta object sought by inum" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 26
      @tsk_meta = Sleuthkit::FileSystem::FileMeta.new(@filesystem, inum)
      ap @tsk_meta.inspect_object
      @tsk_meta.addr.should eq(inum)
      @tsk_meta.should be_an_instance_of Sleuthkit::FileSystem::FileMeta 
    end
  end
  describe "Meta Open by file (file)" do
    it "should return a file meta object sought by tsk_file" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 26
      @directory = @filesystem.find_directory_by_inum(inum)
      @file = @directory.file
      @tsk_meta = Sleuthkit::FileSystem::FileMeta.new(@file)
      ap @tsk_meta.inspect_object
      @tsk_meta.addr.should eq(inum)
      @tsk_meta.should be_an_instance_of Sleuthkit::FileSystem::FileMeta 
    end
  end
  describe "Meta Open by dir (dir)" do
    it "should return a file meta object sought by tsk_dir" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 26
      @directory = @filesystem.find_directory_by_inum(inum)
      @tsk_meta = Sleuthkit::FileSystem::FileMeta.new(@directory)
      ap @tsk_meta.inspect_object
      @tsk_meta.addr.should eq(inum)
      @tsk_meta.should be_an_instance_of Sleuthkit::FileSystem::FileMeta 
    end
  end
  # describe "File Open (name)" do
  #   it "should return a file object sought by name" do
  #     @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
  #     name = 'Test_Root_Folder'
  #     @tsk_file = Sleuthkit::FileSystem::FileData.new(@filesystem, name)
  #     @dir.should be_an_instance_of Sleuthkit::FileSystem::FileData 
  #   end
  # end
  
  
end