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
  end
  
  describe "Directory Open by inum (inum)" do
    it "should return a directory listing sought by inum" do
      @filesystem = Sleuthkit::FileSystem.new(@mac_fs_only_image)
      name = '/Test_Root_Folder'
      inum = 26
      puts inum.class
      @dir = @filesystem.find_directory_by_inum(inum)
      puts @dir.class
      @dir.should be_an_instance_of Sleuthkit::FileSystemDirectory 
    end
  end
  describe "Directory Open (name)" do
    it "should return a directory listing sought by name" do
      @filesystem = Sleuthkit::FileSystem.new(@mac_fs_only_image)
      name = 'Test_Root_Folder'
      @dir = @filesystem.find_directory_by_name(name)
      @dir.should be_an_instance_of Sleuthkit::FileSystemDirectory 
    end
  end
  
  
end