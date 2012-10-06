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
  
  describe "FileSystem#find_directory_by_inum (inum)" do
    it "should return a directory listing sought by inum" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 26
      puts inum.class

      @dir = @filesystem.find_directory_by_inum(inum)
      puts @dir.class
      @dir.should be_an_instance_of Sleuthkit::FileSystem::Directory 
      @dir.inum.should eq(26)
    end
  end
  describe "FileSystem#find_directory_by_name (name)" do
    it "should return a directory listing sought by name" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      name = 'Test_Root_Folder'

      @dir = @filesystem.find_directory_by_name(name)
      @dir.should be_an_instance_of Sleuthkit::FileSystem::Directory 
      @dir.name.should eq(name)
    end
  end
  describe "FileSystem#find_directory (inum)" do
    it "should return a directory listing sought by name" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      inum = 26

      @dir = @filesystem.find_directory(inum)
      @dir.should be_an_instance_of Sleuthkit::FileSystem::Directory 
      @dir.inum.should eq(inum)
    end
  end
  describe "FileSystem#find_directory (name)" do
    it "should return a directory listing sought by name" do
      @filesystem = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      name = 'Test_Root_Folder'

      @dir = @filesystem.find_directory(name)
      @dir.should be_an_instance_of Sleuthkit::FileSystem::Directory 
      @dir.name.should eq(name)
    end
  end
  
  
end