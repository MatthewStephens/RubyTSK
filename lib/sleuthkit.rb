# -*- coding: utf-8 -*-
require 'sleuthkit/image'
require 'sleuthkit/file_system'
require 'sleuthkit/version'
require 'sleuthkit/volume'
require 'tsk4r/tsk4r' # gem install process uses this dir
# require 'sleuthkit/image.rb'

module Sleuthkit
  # require 'ruby-debug'
  require 'pp'
  
  def self.open(path, opts={})
    image = nil
    case path
    when String
      image = Sleuthkit::Image.new(path, opts)
    when Array
      image = Sleuthkit::Image.new(path, opts)
    else
      return nil
    end
    if image == nil
      return nil
    else
      volume = Sleuthkit::VolumeSystem.new(image)
      if ! volume.tainted?
        image.volumes = []
        image.volumes << volume
        filesystem = Sleuthkit::FileSystem.new(volume)
        if ! filesystem.tainted?
          image.filesystems = []
          image.filesystems << filesystem
        end
      else
        image.volumes = nil
        filesystem = Sleuthkit::FileSystem.new(image)
        if ! filesystem.tainted?
          image.filesystems = []
          image.filesystems << filesystem
        else
          image.filesystems = nil
          # more to do here
        end
      end
    end
    return image
  end
  
  # tell us about yourself
  def self.version_string
    "Sleuthkit Ruby Binding version #{Sleuthkit::VERSION} (TSK version: #{Sleuthkit::TSK_VERSION})"
  end
  def [](sym)
    self.instance_variable_get("@#{sym.to_s}")
  end
  def inspect_object
    #debugger
    h = Hash.new
    self.instance_variables.each do |var|
      h[var.to_s.gsub(/@/, '').to_sym] = instance_variable_get(var)
    end
    h
  end

end
