import React from 'react';
import { ChevronLeft, ChevronRight, Maximize } from 'lucide-react';

export default function Controls({ currentSlide, totalSlides, prevSlide, nextSlide }) {
  const handleFullscreen = () => {
    if (!document.fullscreenElement) {
      document.documentElement.requestFullscreen();
    } else {
      document.exitFullscreen();
    }
  };

  return (
    <div className="mt-6 flex items-center justify-center space-x-6 z-20 relative">
      <button 
        onClick={prevSlide}
        disabled={currentSlide === 0}
        className="p-3 rounded-full bg-gray-800 text-white hover:bg-blue-600 disabled:opacity-30 disabled:cursor-not-allowed transition"
      >
        <ChevronLeft size={24} />
      </button>
      
      <div className="text-gray-400 text-sm">
        Sử dụng <kbd className="bg-gray-800 px-2 py-1 rounded border border-gray-700 mx-1">←</kbd> <kbd className="bg-gray-800 px-2 py-1 rounded border border-gray-700 mx-1">→</kbd> để chuyển slide
      </div>
      
      <button 
        onClick={nextSlide}
        disabled={currentSlide === totalSlides - 1}
        className="p-3 rounded-full bg-gray-800 text-white hover:bg-blue-600 disabled:opacity-30 disabled:cursor-not-allowed transition"
      >
        <ChevronRight size={24} />
      </button>

      <div className="h-6 w-px bg-gray-700 mx-2"></div>

      <button
        onClick={handleFullscreen}
        className="p-3 rounded-full bg-gray-800 text-white hover:bg-blue-600 transition"
        title="Toàn màn hình"
      >
        <Maximize size={22} />
      </button>
    </div>
  );
}
