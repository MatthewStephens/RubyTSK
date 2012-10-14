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
  describe "File Block get (filesystem, block_addr)" do
    it "should return a file block sought by block address" do
      @addr = 3582
      find_this = "this is a test txt file.\nIt has two lines."
      @filesystem = Sleuthkit::FileSystem::System.new(@volume)
      @block = Sleuthkit::FileSystem::Block.new(@filesystem, @addr)
      @block.should be_an_instance_of Sleuthkit::FileSystem::Block
      @block.filesystem.should eq(@filesystem)
      @block.buffer.should match(find_this)
    end
  end
  
end