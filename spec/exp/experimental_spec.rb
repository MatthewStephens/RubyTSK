describe "spec/experiments", "Sleuthkit FileSystem Experiments" do
  require 'sleuthkit'
  
  before :each do
    @sample_dir="samples"
    
    @mac_fs_only_image_path = "#{@sample_dir}/tsk4r_img_02.dmg"
    @mac_partitioned_image_path = "#{@sample_dir}/tsk4r_img_01.dmg"
    @split_image_files = Dir.glob("#{@sample_dir}/tsk4r*split*a?")
    @split_image = Sleuthkit::Image.new(@split_image_files)
    
    puts "File #{@mac_fs_only_image_path} not found!!" unless File.exist?(@mac_fs_only_image_path)
    puts "File #{@mac_partitioned_image_path} not found!!" unless File.exist?(@mac_partitioned_image_path)
  
    @mac_fs_only_image = Sleuthkit::Image.new(@mac_fs_only_image_path)
    @mac_partitioned_image = Sleuthkit::Image.new(@mac_partitioned_image_path)
  
    @volume = Sleuthkit::VolumeSystem.new(@mac_partitioned_image)
    @string = "some string"
  end

  describe "Experiment 1" do
    it "Should do something funky" do
      puts "hmm"
      @file_system = Sleuthkit::FileSystem.new(@mac_fs_only_image)
      @file_system.block_size.should eq(4096)
      File.open("tempfile-#{$$}.txt", 'w') do |file|
        #file.puts @file_system.public_methods(false).sort
        @file_system.call_tsk_fsstat(file)
      end
      # `ls *.txt`
      #File.delete(file)
      # `ls *.txt`
    end
  end
  
  describe "Experiment 2" do
    it "Should call TSK report function" do
      puts "mmm?"
      @file_system = Sleuthkit::FileSystem.new(@mac_fs_only_image)
      @file_system.block_size.should eq(4096)
      find_this = "Last Backup Date: 	Wed Dec 31 19:00:00 1969"
      result=""
      @file_system.print_tsk_fsstat(result)
      puts result
      result.should match(find_this)
    end
  end
end