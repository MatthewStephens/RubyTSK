describe "spec/experiments", "Sleuthkit FileSystem Experiments" do
  require 'sleuthkit'
  
  before :each do
    @sample_dir="samples"
    
    @mac_fs_only_image_path = "#{@sample_dir}/tsk4r_img_02.dmg"
    @mac_partitioned_image_path = "#{@sample_dir}/tsk4r_img_01.dmg"
    @split_image_files = Dir.glob("#{@sample_dir}/tsk4r_img_01*split.?")
    @split_image = Sleuthkit::Image.new(@split_image_files)
    
    puts "File #{@mac_fs_only_image_path} not found!!" unless File.exist?(@mac_fs_only_image_path)
    puts "File #{@mac_partitioned_image_path} not found!!" unless File.exist?(@mac_partitioned_image_path)
  
    @mac_fs_only_image = Sleuthkit::Image.new(@mac_fs_only_image_path)
    @mac_partitioned_image = Sleuthkit::Image.new(@mac_partitioned_image_path)
  
    @volume = Sleuthkit::Volume::System.new(@mac_partitioned_image)
    @string = "some string"
  end

  describe "Experiment 1" do
    it "Should do something funky" do
      @file_system = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      @file_system.block_size.should eq(4096)
      # File.open("tempfile-#{$$}.txt", 'w') do |file|
      #   #file.puts @file_system.public_methods(false).sort
      #   @file_system.call_tsk_fsstat(file)
      # end
      # `ls *.txt`
      #File.delete(file)
      # `ls *.txt`
    end
  end
  
  describe "Experiment 2" do
    it "Should call TSK filesystem report function" do
      @file_system = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      @file_system.block_size.should eq(4096)
      find_this = "Last Backup Date:   Wed Dec 31 19:00:00 1969"
      result=""
      @file_system.print_tsk_fsstat(result)
      puts result
      result.should match(find_this)
    end
  end
  describe "Experiment 3" do
    it "Should call TSK filesystem report function via the alias #stat" do
      @file_system = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      @file_system.block_size.should eq(4096)
      find_this = "Last Backup Date:   Wed Dec 31 19:00:00 1969"
      result=""
      @file_system.stat(result)
      puts result
      result.should match(find_this)
    end
  end
  describe "Experiment 4" do
    it "Should call TSK file report function, given an inum" do
      @file_system = Sleuthkit::FileSystem::System.new(@mac_fs_only_image)
      find_this = "Type:  File\nMode:  r---------\nSize:  159744"
      inum=3; result=""
      @file_system.istat(inum, result)
      # puts result
      result.should match(find_this)
    end
  end
  describe "Experiment 5" do
    it "Should call TSK file report function, given an inum and a number of blocks" do
      @file_system = Sleuthkit::FileSystem::System.new(@volume)
      find_this = "Data Fork Blocks:\n3597 3598 3599 3600 3601 3602 3603 3604 \n3605 3606 3607 3608 3609 3610 3611 3612 \n"
      inum=42; result=""
      @file_system.istat(inum, result, :block => 5)
      # puts result
      result.should match(find_this)
    end
  end
end