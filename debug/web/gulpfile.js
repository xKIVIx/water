const gulp   = require('gulp');
gulp.task('compile', ['compileHTML', 'compileJS', 'compileCSS'], function() {});

// js compile
const jshint = require('gulp-jshint');
var webpack = require('webpack-stream');

gulp.task('compileJS', ['checkJS'], function() {
    return gulp.src('src/js/index.js')
               .pipe( webpack( require("./webpack.config.js") ) )
               .pipe( gulp.dest('compile') );
});

gulp.task('checkJS', function() {
    return gulp.src('src/js/*.js')
               .pipe( jshint() )
               .pipe( jshint.reporter('default', {esversion: 6}) );
});

var jsdoc = require('gulp-jsdoc3');

gulp.task('doc', function (cb) {
    var config = require('./jsdoc.json');
    gulp.src(['README.md', './src/js/*.js'], {read: false})
        .pipe(jsdoc(config, cb));
});

// html compile
const htmlmin = require('gulp-htmlmin');
const htmlhint = require('gulp-htmlhint');
const inject  = require('gulp-inject');

gulp.task('compileHTML', function() {
    return gulp.src('src/index.html')
               .pipe( htmlhint() )
               .pipe( inject(gulp.src(['src/shaders/*.glsl']), {
                        starttag: '<!-- inject:{{path}} -->',
                        relative: true,
                        removeTags: true,
                        transform: function (filePath, file) {
                            return file.contents.toString('utf8')
                        }}))
               .pipe( htmlmin({collapseWhitespace: true}) )
               .pipe( gulp.dest('compile') );
});

// css compile
const concatCss = require ('gulp-concat-css');
const minCss = require ('gulp-cssmin')
gulp.task('compileCSS', function() {
    return gulp.src('src/css/*.css')
               .pipe( concatCss('style.css') )
               .pipe( minCss() )
               .pipe( gulp.dest('compile') );
});