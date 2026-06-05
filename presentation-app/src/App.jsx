import React, { useState, useEffect, useCallback } from 'react';
import { Presentation } from 'lucide-react';
import { slidesData } from './data/slidesData';
import SlideRenderer from './components/SlideRenderer';
import NotesPanel from './components/NotesPanel';
import Controls from './components/Controls';

export default function App() {
  const [currentSlide, setCurrentSlide] = useState(0);
  const [seconds, setSeconds] = useState(0);

  useEffect(() => {
    const timer = setInterval(() => {
      setSeconds(prev => prev + 1);
    }, 1000);

    return () => clearInterval(timer);
  }, []);

  const formatTime = (s) => {
    const mins = Math.floor(s / 60);
    const secs = s % 60;
    return `${mins}:${secs.toString().padStart(2, '0')}`;
  };

  const nextSlide = useCallback(() => {
    setCurrentSlide((prev) => (prev < slidesData.length - 1 ? prev + 1 : prev));
  }, []);

  const prevSlide = useCallback(() => {
    setCurrentSlide((prev) => (prev > 0 ? prev - 1 : prev));
  }, []);

  useEffect(() => {
    const handleKeyDown = (e) => {
      if (e.key === 'ArrowRight' || e.key === ' ') nextSlide();
      if (e.key === 'ArrowLeft') prevSlide();
    };
    window.addEventListener('keydown', handleKeyDown);
    return () => window.removeEventListener('keydown', handleKeyDown);
  }, [nextSlide, prevSlide]);

  return (
    <div className="min-h-screen bg-gray-900 flex flex-col font-sans">
      
      {/* Header Panel */}
      <div className="bg-gray-800 border-b border-gray-700 h-14 flex items-center justify-between px-6 shrink-0">
        <div className="flex items-center space-x-2">
          <Presentation className="text-blue-400" size={20} />
          <span className="text-gray-200 font-semibold">Chế độ Thuyết trình (Presenter View)</span>
        </div>
        
        <div className="flex items-center space-x-6">
          <div className="text-yellow-400 font-mono text-lg font-medium flex items-center space-x-2">
            <span>⏱</span> 
            <span>{formatTime(seconds)}</span>
          </div>
          <div className="text-gray-400 text-sm font-mono">
            Slide {currentSlide + 1} / {slidesData.length}
          </div>
        </div>
      </div>

      <div className="flex-1 flex p-6 gap-6 overflow-hidden">
        
        {/* Main Projection Area */}
        <div className="flex-1 flex flex-col items-center justify-center relative">
          <div 
            className="w-full max-w-5xl aspect-video rounded-lg shadow-2xl overflow-hidden relative transition-all duration-500 ease-in-out border border-white/10"
            style={{
              background: 'linear-gradient(135deg, #020617 0%, #0f172a 30%, #1d4ed8 70%, #0c4a6e 100%)',
              boxShadow: '0 25px 50px -12px rgba(0, 0, 0, 0.5)'
            }}
          >
            {/* Grid Pattern Background */}
            <div className="absolute inset-0 opacity-[0.03] bg-[linear-gradient(rgba(255,255,255,1)_1px,transparent_1px),linear-gradient(90deg,rgba(255,255,255,1)_1px,transparent_1px)] bg-[size:40px_40px] pointer-events-none"></div>
            
            {/* Render Slide */}
            <SlideRenderer slide={slidesData[currentSlide]} currentSlide={currentSlide} />

            {/* Pagination dots */}
            <div className="absolute bottom-4 left-0 right-0 flex justify-center space-x-2 z-20 pointer-events-none">
              {slidesData.map((_, idx) => (
                <div 
                  key={idx} 
                  className={`h-1.5 rounded-full transition-all duration-300 ${idx === currentSlide ? 'w-6 bg-yellow-400' : 'w-1.5 bg-white/30'}`}
                />
              ))}
            </div>
          </div>

          {/* Controls Bar */}
          <Controls 
            currentSlide={currentSlide} 
            totalSlides={slidesData.length} 
            prevSlide={prevSlide} 
            nextSlide={nextSlide} 
          />
        </div>

        {/* Presenter Notes / AI Panel */}
        <NotesPanel currentSlide={currentSlide} />

      </div>
    </div>
  );
}
