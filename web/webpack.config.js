const UglifyJsPlugin = require('uglifyjs-webpack-plugin')

module.exports = {
    output: {
        filename: "script.js"
    },
    plugins: [
        new UglifyJsPlugin({
            sourceMap: true
        })
    ],
    devtool: 'source-map',
    module : {
        loaders: [ { 
                test   : /.js$/,
                loader : 'babel-loader'
            }
        ]
    }
}