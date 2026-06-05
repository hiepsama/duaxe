import React, { useState } from 'react';
import { motion, AnimatePresence } from 'framer-motion';
import { Anchor, Map as MapIcon, BookOpen, Shield, PlayCircle, Image as ImageIcon, MessageCircleQuestion } from 'lucide-react';

const iconMap = {
  Anchor, MapIcon, BookOpen, Shield
};

export default function SlideRenderer({ slide, currentSlide }) {
  const [laser, setLaser] = useState({ x: 0, y: 0 });

  const renderSlideContent = () => {
    switch (slide.type) {
      case 'cover':
        return (
          <div className="flex flex-col items-center justify-center h-full text-center space-y-6 relative z-10">
            <h1 className="text-5xl font-black text-white leading-tight drop-shadow-lg whitespace-pre-line uppercase">
              {slide.title}
            </h1>
            <div className="w-24 h-1 bg-yellow-400 rounded-full"></div>
            <p className="text-2xl text-blue-100 font-medium drop-shadow">
              {slide.subtitle}
            </p>
          </div>
        );
      
      case 'icons':
        return (
          <div className="flex flex-col h-full pt-8 px-12 relative z-10">
            <h2 className="text-4xl font-bold text-white mb-12 drop-shadow-md border-b-2 border-blue-400 pb-4 inline-block">{slide.title}</h2>
            <div className="grid grid-cols-2 gap-8 mt-4">
              {slide.bullets.map((item, idx) => {
                const Icon = iconMap[item.iconName] || Anchor;
                return (
                  <div key={idx} className="bg-white/10 backdrop-blur-md rounded-xl p-6 flex items-center space-x-6 border border-white/20 hover:bg-white/20 transition duration-300">
                    <div className="bg-blue-500/50 p-4 rounded-full text-white">
                      <Icon size={40} />
                    </div>
                    <span className="text-2xl text-white font-medium">{item.text}</span>
                  </div>
                );
              })}
            </div>
          </div>
        );

      case 'video':
        return (
           <div className="flex flex-col items-center justify-center h-full relative z-10">
             <h2 className="text-4xl font-bold text-white mb-12 drop-shadow-md">{slide.title}</h2>
             <div className="w-3/4 aspect-video bg-black rounded-xl border border-white/20 flex flex-col items-center justify-center text-white/70 shadow-2xl relative overflow-hidden group">
                <video autoPlay controls className="w-full h-full object-cover">
                  {/* Using a placeholder since we don't have the real video */}
                  <source src="/videos/truongsa.mp4" type="video/mp4" />
                </video>
             </div>
           </div>
        );

      case 'qa':
        return (
           <div className="flex flex-col items-center justify-center h-full text-center space-y-8 relative z-10">
             <MessageCircleQuestion size={80} className="text-yellow-400" />
             <h2 className="text-5xl font-bold text-white drop-shadow-md">{slide.title}</h2>
             <div className="bg-white/10 backdrop-blur-md p-8 rounded-2xl border border-white/20 max-w-3xl mt-8">
                <p className="text-3xl text-yellow-300 font-semibold italic">
                  "{slide.bullets[0]}"
                </p>
             </div>
           </div>
        );

      default: // content, split, timeline, situation, etc.
        return (
          <div className="flex flex-col h-full pt-8 px-12 relative z-10">
            <h2 className="text-4xl font-bold text-white mb-8 drop-shadow-md border-b-2 border-blue-400 pb-4 inline-block">{slide.title}</h2>
            <div className="flex flex-row h-full gap-8">
              {/* Text Column */}
              <div className={`flex flex-col justify-center space-y-6 ${slide.type === 'content' ? 'w-full' : 'w-1/2'}`}>
                {slide.bullets.map((bullet, idx) => {
                   const isQuote = slide.type === 'quote' && idx === slide.bullets.length - 1;
                   return (
                    <div key={idx} className={`flex items-start ${isQuote ? 'mt-8 bg-blue-900/50 p-6 rounded-xl border border-yellow-400/50' : ''}`}>
                      {!isQuote && <div className="min-w-3 w-3 h-3 rounded-full bg-yellow-400 mt-2.5 mr-4 shadow-[0_0_8px_rgba(250,204,21,0.8)]"></div>}
                      <p className={`text-2xl text-white/95 leading-relaxed ${isQuote ? 'text-yellow-300 italic font-medium' : ''}`}>
                        {bullet}
                      </p>
                    </div>
                   )
                })}
              </div>
              
              {/* Image Hint Column */}
              {slide.type !== 'content' && (
                <div className="w-1/2 flex items-center justify-center p-6">
                  <div className="w-full h-full min-h-[300px] bg-blue-950/40 rounded-2xl border-2 border-dashed border-blue-300/30 flex flex-col items-center justify-center text-blue-200/60 p-8 text-center backdrop-blur-sm relative overflow-hidden">
                    <ImageIcon size={64} className="mb-4 opacity-50" />
                    <p className="text-lg font-medium">{slide.imageHint}</p>
                    <div className="absolute top-4 right-4 text-xs font-mono bg-blue-900 px-3 py-1 rounded-full opacity-80">
                      Gợi ý Hình ảnh
                    </div>
                  </div>
                </div>
              )}
            </div>
          </div>
        );
    }
  };

  return (
    <div 
      className="absolute inset-0 overflow-hidden cursor-crosshair"
      onMouseMove={(e) => {
        setLaser({
          x: e.nativeEvent.offsetX,
          y: e.nativeEvent.offsetY
        });
      }}
    >
      <AnimatePresence mode="wait">
        <motion.div
          key={currentSlide}
          initial={{ opacity: 0, x: 80 }}
          animate={{ opacity: 1, x: 0 }}
          exit={{ opacity: 0, x: -80 }}
          transition={{ duration: 0.45 }}
          className="absolute inset-0"
        >
          {renderSlideContent()}
        </motion.div>
      </AnimatePresence>

      {/* Laser Pointer */}
      <div
        className="absolute w-4 h-4 bg-red-500 rounded-full pointer-events-none blur-[1px] shadow-[0_0_10px_2px_rgba(239,68,68,0.8)] z-50 transition-transform duration-75 ease-out"
        style={{
          left: laser.x - 8,
          top: laser.y - 8,
          boxShadow: '0 0 10px 2px rgba(239, 68, 68, 0.8), 0 0 20px 5px rgba(239, 68, 68, 0.5)'
        }}
      />
    </div>
  );
}
