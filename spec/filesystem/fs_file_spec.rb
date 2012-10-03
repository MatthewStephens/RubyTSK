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
    
    @volume = Sleuthkit::Volume::System.new(@mac_partitioned_image)
  end
  
  # use `icat mac_fs_only_image_path inum` to troubleshoot
  describe "File Open by inum (inum)" do
    it "should return a directory listing sought by inum" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 35
      puts inum.class
      @tsk_file = Sleuthkit::FileSystem::FileData.new(@filesystem, inum)
      @tsk_file.name.should match("README")
      @tsk_file.should be_an_instance_of Sleuthkit::FileSystemFileData 
    end
  end
  # describe "File Open (name)" do
  #   it "should return a directory listing sought by name" do
  #     @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
  #     name = 'Test_Root_Folder'
  #     @tsk_file = Sleuthkit::FileSystem::FileData.new(@filesystem, name)
  #     @dir.should be_an_instance_of Sleuthkit::FileSystem::FileData 
  #   end
  # end
  
  
end