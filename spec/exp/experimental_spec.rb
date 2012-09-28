describe 'spec/experiments', "Sleuthkit FileSystem Experiments" do
  require 'sleuthkit'
  
  before :each do
    @linux_image_path = "spec/samples/linux.iso"
    @mac_image_path = "spec/samples/tsk4r_img_01.dmg"
    @split_image_files = Dir.glob("spec/samples/tsk4r*split*a?")
    @split_image = Sleuthkit::Image.new(@split_image_files)
    
    puts "File #{@linux_image_path} not found!!" unless File.exist?(@linux_image_path)
    puts "File #{@mac_image_path} not found!!" unless File.exist?(@mac_image_path)
  
    @linux_image = Sleuthkit::Image.new(@linux_image_path)
    @mac_image = Sleuthkit::Image.new(@mac_image_path)
  
    @volume = Sleuthkit::VolumeSystem.new(@mac_image)
    @string = "some string"
  end

  describe "Experiment 1" do
    it "Should do something funky" do
      @file_system = Sleuthkit::FileSystem.new(@linux_image)
      @file_system.block_size.should eq(2048)
      File.open("tempfile-#{$$}.txt", 'w') do |file|
#        pp @file_system.public_methods(false).sort
        @file_system.call_tsk_fsstat(file)
      end
      `ls *.txt`
      #File.delete(file)
      `ls *.txt`
    end
  end
  
  describe "Experiment 2" do
    it "Should call TSK report function" do
      @file_system = Sleuthkit::FileSystem.new(@linux_image)
      @file_system.block_size.should eq(2048)
      find_this = "Recording Application: MKISOFS ISO 9660/HFS FILESYSTEM BUILDER"
      result=String.new
      @file_system.print_tsk_fsstat(result)
      result.should match(find_this)
      #puts result
    end
  end
end